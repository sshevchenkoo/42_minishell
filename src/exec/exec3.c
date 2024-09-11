/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ukireyeu <ukireyeu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:20:34 by ukireyeu          #+#    #+#             */
/*   Updated: 2024/09/11 12:12:32 by ukireyeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

void	handle_fork_exec(t_tree *node, t_env *env, int input_fd, int *stat)
{
	int	pid;

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
		if (node->fd != -1)
		{
			dup2(node->fd, STDOUT_FILENO);
			close(node->fd);
		}
		basic_exec(node, env);
	}
	if (input_fd != -1)
		close(input_fd);
	waitpid(pid, stat, 0);
}

void	close_and_exec_pipe(t_tree *n, t_env *e, int ifd, int *s)
{
	traverse_and_execute(n->left, e, ifd, s);
	exit(EXIT_SUCCESS);
}

void	create_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

void	handle_pipe(t_tree *node, t_env *env, int input_fd, int *stat)
{
	int	pipefd[2];
	int	pid;

	create_pipe(pipefd);
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
		close_and_exec_pipe(node, env, input_fd, stat);
	}
	if (input_fd != -1)
		close(input_fd);
	close(pipefd[1]);
	traverse_and_execute(node->right, env, pipefd[0], stat);
	waitpid(pid, stat, 0);
	close(pipefd[0]);
}

void	handle_redirection(t_tree *node, t_env *env, int input_fd, int *stat)
{
	int	fd;
	int	flags;

	flags = O_RDWR;
	if (node->type == REDIR_OUT)
		flags = O_RDWR | O_CREAT | O_TRUNC;
	else
		flags = O_RDWR | O_APPEND | O_CREAT;
	fd = open(node->right->content[0], flags, 0644);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	node->left->fd = fd;
	handle_fork_exec(node->left, env, input_fd, stat);
}
