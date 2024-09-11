/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ukireyeu <ukireyeu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:08:53 by neulad            #+#    #+#             */
/*   Updated: 2024/09/10 19:56:45 by ukireyeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*add_char(char *str, char c)
{
	char	*res;
	int		len;
	int		clen;

	len = ft_strlen(str);
	clen = len;
	res = malloc(len + 2);
	if (!res)
		return (NULL);
	while (len--)
		res[len] = str[len];
	res[clen] = c;
	res[clen + 1] = '\0';
	free(str);
	return (res);
}

static char	*get_var(char *str, t_env *env)
{
	char	*var_name;
	int		i;

	var_name = ft_strdup("");
	if (!var_name)
		return (NULL);
	if (*str == '?')
		var_name = "?";
	while (*str && validchar(*str))
	{
		var_name = add_char(var_name, *str);
		++str;
	}
	i = find_var_env(env, var_name);
	if (var_name[0] != '?')
		free(var_name);
	if (i == -1)
		return (NULL);
	return (env->parsed_env[i][1]);
}

static char	*handle_variable(char **str, t_env *env, char *res)
{
	char	*varname;

	++(*str);
	varname = get_var(*str, env);
	if (!varname)
	{
		skipvar(str);
		return (res);
	}
	res = ft_strjoin(res, varname);
	skipvar(str);
	return (res);
}

static int	toggle_quotes(char c, int *dquote, int *squote)
{
	if (c == '"' && !(*squote))
	{
		*dquote = !(*dquote);
		return (1);
	}
	else if (c == '\'' && !(*dquote))
	{
		*squote = !(*squote);
		return (1);
	}
	return (0);
}

char	*expand_quotes(char *str, t_env *env)
{
	char	*res;
	int		dquote;
	int		squote;

	dquote = 0;
	squote = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (*str)
	{
		if (toggle_quotes(*str, &dquote, &squote))
			;
		else if (*str == '$' && !squote)
		{
			res = handle_variable(&str, env, res);
			continue ;
		}
		else
			res = add_char(res, *str);
		++str;
	}
	return (res);
}
