/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ukireyeu <ukireyeu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:16:21 by ukireyeu          #+#    #+#             */
/*   Updated: 2024/09/15 15:48:44 by ukireyeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

void	handle_heredoc_input(char **heredoc_input, t_tree *node)
{
	char	*heredoc_line;
	char	*tmp;

	*heredoc_input = ft_strdup("");
	heredoc_line = readline("heredoc> ");
	while (heredoc_line)
	{
		heredoc_line = readline("heredoc> ");
		if (!ft_strncmp(heredoc_line,
				node->right->content[0], ft_strlen(node->right->content[0])))
			break ;
		tmp = *heredoc_input;
		*heredoc_input = ft_strjoin(*heredoc_input, "\n");
		free(tmp);
		tmp = *heredoc_input;
		*heredoc_input = ft_strjoin(*heredoc_input, heredoc_line);
		free(tmp);
	}
}

void	close_and_exec_heredoc(int pfd[2], t_tree *node, t_env *env, int *stat)
{
	close(pfd[1]);
	traverse_and_execute(node->left, env, pfd[0], stat);
	exit(EXIT_SUCCESS);
}

void	create_pipe_or_exit(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

void	handle_heredoc(t_tree *node, t_env *env, int *stat)
{
	char	*heredoc_input;
	int		pipefd[2];
	int		pid;

	handle_heredoc_input(&heredoc_input, node);
	create_pipe_or_exit(pipefd);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		close_and_exec_heredoc(pipefd, node, env, stat);
	close(pipefd[0]);
	print_string_to_fd(heredoc_input, pipefd[1]);
	close(pipefd[1]);
	waitpid(pid, stat, 0);
	*stat = WEXITSTATUS(*stat);
	free(heredoc_input);
}

void	traverse_and_execute(t_tree *node, t_env *env, int input_fd, int *stat)
{
	quote_handle(node->content, env);
	node->fd = -1;
	if (node->type == WORD)
	{
		if (check_builtin(node->content[0]))
			handle_builtin(node, env, stat);
		else
			handle_fork_exec(node, env, input_fd, stat);
	}
	else if (node->type == PIPE)
		handle_pipe(node, env, input_fd, stat);
	else if (node->type == REDIR_OUT || node->type == APPEND)
		handle_redirection(node, env, input_fd, stat);
	else if (node->type == REDIR_IN)
		handle_fork_exec(node->left, env,
			open(node->right->content[0], O_RDONLY), stat);
	else if (node->type == HEREDOC)
		handle_heredoc(node, env, stat);
}
