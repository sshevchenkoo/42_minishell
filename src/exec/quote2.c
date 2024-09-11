/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ukireyeu <ukireyeu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:49:28 by ukireyeu          #+#    #+#             */
/*   Updated: 2024/09/10 19:56:58 by ukireyeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	validchar(char c)
{
	return (ft_isalnum(c) || c == '_');
}

void	skipvar(char **str)
{
	while (**str && validchar(**str))
		++(*str);
	if (**str == '?')
		++(*str);
}
