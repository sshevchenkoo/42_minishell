/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yashevch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:09:50 by yashevch          #+#    #+#             */
/*   Updated: 2024/07/08 12:10:13 by yashevch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/minishell.h"

void	count_quotes(const char *l, int *s, int *d)
{
	int	i;

	i = 0;
	while (l[i] != '\0')
	{
		if (l[i] == 34)
			(*d)++;
		if (l[i] == 39)
			(*s)++;
		i++;
	}
}

void	update_quotes(char a, int *s, int *d)
{
	if (a == 34)
		(*d)++;
	if (a == 39)
		(*s)++;
}

int	is_invl(const char **line)
{
	char	start_operator;

	start_operator = **line;
	(*line)++;
	if (start_operator == **line)
		(*line)++;
	while (**line && (**line == 32 || **line == '\t'))
		(*line)++;
	if (**line == '\0' || **line == 60 || **line == 62 || **line == 124)
		return (1);
	return (0);
}
