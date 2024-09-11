/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ukireyeu <ukireyeu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:04:28 by ukireyeu          #+#    #+#             */
/*   Updated: 2024/09/11 12:12:57 by ukireyeu         ###   ########.fr       */
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

	argv_dup = arr2d_dup(node->content);
	if (!argv_dup)
		return ;
	if (access(node->content[0], F_OK | X_OK) == 0)
	{
		free(argv_dup[0]);
		argv_dup[0] = get_filename(node->content[0]);
		path = node->content[0];
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

void	quote_handle(char **content, t_env *env)
{
	char	*tmp;

	if (!content)
		return ;
	while (*content)
	{
		tmp = *content;
		*content = expand_quotes(*content, env);
		free(tmp);
		++content;
	}
}

void	handle_builtin(t_tree *node, t_env *env, int *stat)
{
	int	len;

	len = ft_strlen(node->content[0]);
	if (ft_strncmp(node->content[0], "echo", len) == 0)
		echo_cmd(node->content, STDOUT_FILENO);
	else if (ft_strncmp(node->content[0], "cd", len) == 0)
		cd_cmd(node->content, env, STDOUT_FILENO);
	else if (ft_strncmp(node->content[0], "pwd", len) == 0
		|| ft_strncmp(node->content[0], "env", len) == 0)
		env_or_pwd(node->content[0], env, STDOUT_FILENO);
	else if (ft_strncmp(node->content[0], "unset", len) == 0
		|| ft_strncmp(node->content[0], "export", len) == 0)
		unset_or_export(node->content, env, STDOUT_FILENO, stat);
	else if (ft_strncmp(node->content[0], "exit", len) == 0)
		builtin_exit(node->content);
}
