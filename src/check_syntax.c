/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yashevch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:16:14 by yashevch          #+#    #+#             */
/*   Updated: 2024/07/07 22:52:31 by yashevch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/minishell.h"

int	check_double_operator(const char *l)
{
	int	single_q;
	int	double_q;

	single_q = 0;
	double_q = 0;
	while (*l)
	{
		update_quotes(*l, &single_q, &double_q);
		if (((*l == '|' && *(l + 1) == '|') || (*l == '&' && *(l + 1) == '&'))
			&& !(single_q % 2) && !(double_q % 2))
			return (1);
		l++;
	}
	return (0);
}

int	check_operator_mis(const char *l, int single_q, int double_q, int flag)
{
	while (*l && (*l == 32 || *l == '\t'))
		l++;
	if (*l == '|' || *l == '&')
		return (1);
	while (*l)
	{
		update_quotes(*l, &single_q, &double_q);
		if (*l == '|' && !(single_q % 2) && !(double_q % 2))
		{
			if (flag)
				return (1);
			flag = 1;
		}
		else if (*l != 32 && *l != '\t')
			flag = 0;
		l++;
	}
	if (flag)
		return (1);
	return (0);
}

int	check_invalid_red(const char *l)
{
	int	single_q;
	int	double_q;

	single_q = 0;
	double_q = 0;
	while (*l)
	{
		update_quotes(*l, &single_q, &double_q);
		if ((*l == 62 || *l == 60) && !(single_q % 2) && !(double_q % 2))
		{
			if (is_invl(&l))
				return (1);
		}
		l++;
	}
	return (0);
}

int	check_unclosed_quotes(const char *line)
{
	int	single_q;
	int	double_q;

	single_q = 0;
	double_q = 0;
	count_quotes(line, &single_q, &double_q);
	if (single_q % 2 != 0 || double_q % 2 != 0)
		return (1);
	return (0);
}

int	check_syntax(const char *line)
{
	if (check_unclosed_quotes(line))
		return (ft_putstr_fd("Error: unclosed quote\n", 2), 1);
	if (check_invalid_red(line))
		return (ft_putstr_fd("Error: invalid redirections\n", 2), 1);
	if (check_operator_mis(line, 0, 0, 0))
		return (ft_putstr_fd("Error: misplaced operator\n", 2), 1);
	if (check_double_operator(line))
		return (ft_putstr_fd("Error: operator '&&' and '||'\n", 2), 1);
	return (0);
}
