/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ukireyeu <ukireyeu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:52:46 by ukireyeu          #+#    #+#             */
/*   Updated: 2024/09/11 12:52:53 by ukireyeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_parsed_env(t_env *env)
{
	int	i;

	i = 0;
	while (env->parsed_env[i] != NULL)
	{
		printf("%s=", env->parsed_env[i][0]);
		printf("%s\n", env->parsed_env[i][1]);
		i++;
	}
}
