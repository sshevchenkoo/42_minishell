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

void	shell_exec_loop(t_env *env)
{
	char	*input;
	t_token	*tokens;
	t_tree	*tree;

	while (42)
	{
		input = readline("Pip-Pop: ");
		if (input == NULL)
			break ;
		if (input && *input)
			add_history(input);
		tokens = check_and_tokenize(input);
		if (tokens)
		{
			tree =	parse_tokens(&tokens);
			//print_tree(tree, 0);
			//exec_command(tree, env);
			//free_tree(tree);
		}
		//update_env();
	}
}

int	main(int argc, char **argv, char **env)
{
	t_env	*my_env;

	my_env = malloc(sizeof(t_env));
	if (!my_env)
		return (127);
	if (!isatty(0) || !isatty(1))
		return (0);
	if (argc == 1)
	{
		shell_exec_loop(my_env);
	}
	return (0);
}
