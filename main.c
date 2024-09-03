/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yashevch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:02:41 by yashevch          #+#    #+#             */
/*   Updated: 2024/07/03 21:36:19 by yashevch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "includes/minishell.h"

void	cleanup_shell(t_env *env, int status)
{
	if (env)
	{
		free_array(env->env);
		free_env_var(env->parsed_env);
		free(env);
	}
	exit(status);
}

void	shell_exec_loop(t_env *env)
{
	int		status;
	char	*input;
	t_token	*tokens;
	t_tree	*tree;

	while (42)
	{
		status = 0;
		input = readline("Pip-Pop: ");
		if (input == NULL)
			break ;
		if (input && *input)
			add_history(input);
		tokens = check_and_tokenize(input);
		if (!tokens)
			status = 258;
		// print_parsed_env(env);
		if (tokens)
		{
			tree = parse_tokens(&tokens);
			traverse_and_execute(tree, env, -1);
			// print_tree(tree, 0);
			// exec_command(tree, env, &status);
			free_tree(tree);
		}
		update_env(env, status, "?=");
	}
}

int	main(int argc, char **argv, char **env)
{
	t_env	*my_env;

	(void)argv;
	my_env = malloc(sizeof(t_env));
	if (!my_env)
		return (127);
	if (!isatty(0) || !isatty(1))
		return (free(env), 0);
	if (argc == 1 && init_shell_env(my_env, env))
	{
		shell_exec_loop(my_env);
		cleanup_shell(my_env, 0);
	}
	return (0);
}
