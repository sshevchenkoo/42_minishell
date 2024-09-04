/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ukireyeu < ukireyeu@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:04:28 by ukireyeu          #+#    #+#             */
/*   Updated: 2024/08/31 15:48:41 by ukireyeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>

void	form_argv(char **argv, char *correct_path)
{
	free(argv[0]);
	argv[0] = correct_path;
}

int	check_builtin(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (ft_strncmp(str, "echo", len) == 0 || ft_strncmp(str, "cd", len) == 0
		|| ft_strncmp(str, "pwd", len) == 0 || ft_strncmp(str, "export",
			len) == 0 || ft_strncmp(str, "unset", len) == 0 || ft_strncmp(str,
			"env", len) == 0 || ft_strncmp(str, "exit", len) == 0)
		return (1);
	return (0);
}

void	basic_exec(t_tree *node, t_env *env)
{
	char	**argv_dup;
	char	*path;
	int		len;

	argv_dup = arr2d_dup(node->content);
	if (!argv_dup)
		return ;
	if (access(node->content[0], F_OK | X_OK) == 0)
	{
		free(argv_dup[0]);
		argv_dup[0] = get_filename(node->content[0]);
		path = node->content[0];
	}
	else if (check_builtin(node->content[0]))
	{
		len = ft_strlen(node->content[0]);
		if (ft_strncmp(node->content[0], "echo", len) == 0)
			echo_cmd(node->content, STDOUT_FILENO);
		else if (ft_strncmp(node->content[0], "cd", len) == 0)
			cd_cmd(node->content, env, STDOUT_FILENO);
		else if (ft_strncmp(node->content[0], "pwd", len) == 0
			|| ft_strncmp(node->content[0], "env", len) == 0)
			env_or_pwd(node->content[0], env, STDOUT_FILENO);
		exit(EXIT_SUCCESS);
	}
	else
	{
		path = get_path(node->content[0], env->env);
		if (!path)
		{
			perror("path");
			exit(EXIT_FAILURE);
		}
		free(argv_dup[0]);
		argv_dup[0] = path;
	}
	execve(path, argv_dup, env->env);
}

void	traverse_and_execute(t_tree *node, t_env *env, int input_fd)
{
	int		pid;
	int		pipefd[2];
	int		fd;
	char	*heredoc_line;
	char	*heredoc_input;
	char	*tmp;

	if (node->type == WORD)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			if (input_fd != -1)
			{
				dup2(input_fd, STDIN_FILENO);
				close(input_fd);
			}
			basic_exec(node, env);
		}
		if (input_fd != -1)
			close(input_fd);
		waitpid(pid, NULL, 0);
	}
	else if (node->type == PIPE)
	{
		if (node->left && node->left->type == REDIR_OUT)
		{
			traverse_and_execute(node->left, env, input_fd);
			fd = open(node->left->right->content[0], O_RDONLY);
			traverse_and_execute(node->right, env, fd);
			close(fd);
			return ;
		}
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			close(pipefd[0]);
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
			traverse_and_execute(node->left, env, input_fd);
			exit(EXIT_SUCCESS);
		}
		if (input_fd != -1)
			close(input_fd);
		close(pipefd[1]);
		traverse_and_execute(node->right, env, pipefd[0]);
		waitpid(pid, NULL, 0);
		close(pipefd[0]);
	}
	else if (node->type == REDIR_OUT || node->type == APPEND)
	{
		if (node->type == REDIR_OUT)
			fd = open(node->right->content[0], O_RDWR | O_CREAT | O_TRUNC,
					0644);
		else
			fd = open(node->right->content[0], O_RDWR | O_APPEND | O_CREAT,
					0644);
		if (fd == -1)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			if (input_fd != -1)
			{
				dup2(input_fd, STDIN_FILENO);
				close(input_fd);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			traverse_and_execute(node->left, env, input_fd);
			exit(EXIT_SUCCESS);
		}
		close(input_fd);
		close(fd);
		waitpid(pid, NULL, 0);
	}
	else if (node->type == REDIR_IN)
	{
		if (input_fd != -1)
			close(input_fd);
		fd = open(node->right->content[0], O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
		traverse_and_execute(node->left, env, fd);
		close(fd);
	}
	else if (node->type == HEREDOC)
	{
		heredoc_input = ft_strdup("");
		while ((heredoc_line = readline("heredoc> ")))
		{
			if (!ft_strncmp(heredoc_line, node->right->content[0],
					ft_strlen(node->right->content[0])))
			{
				break ;
			}
			tmp = heredoc_input;
			heredoc_input = ft_strjoin(heredoc_input, "\n");
			free(tmp);
			tmp = heredoc_input;
			heredoc_input = ft_strjoin(heredoc_input, heredoc_line);
			free(tmp);
		}
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			close(pipefd[1]);
			traverse_and_execute(node->left, env, pipefd[0]);
			exit(EXIT_SUCCESS);
		}
		close(pipefd[0]);
		print_string_to_fd(heredoc_input, pipefd[1]);
		close(pipefd[1]);
		waitpid(pid, NULL, 0);
		free(heredoc_input);
	}
}

// void	run_pipe(t_tree *ast, char **env)
// {
// 	int	pipe1[2];
// 	int	fork1;
// 	int	fork2;

// 	if (pipe(pipe1) == -1)
// 		return ;
// 	fork1 = fork();
// 	if (fork1 == -1)
// 		return ;
// 	if (fork1 == 0)
// 	{
// 		close(pipe1[0]);
// 		dup2(pipe1[1], STDOUT_FILENO);
// 		close(pipe1[1]);
// 		basic_exec(ast->left, env);
// 	}
// 	fork2 = fork();
// 	if (fork2 == -1)
// 		return ;
// 	if (fork2 == 0)
// 	{
// 		close(pipe1[1]);
// 		dup2(pipe1[0], STDIN_FILENO);
// 		close(pipe1[0]);
// 		basic_exec(ast->right, env);
// 	}
// 	close(pipe1[0]);
// 	close(pipe1[1]);
// 	waitpid(fork1, NULL, 0);
// 	waitpid(fork2, NULL, 0);
// }

