/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ukireyeu <ukireyeu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:08:53 by neulad            #+#    #+#             */
/*   Updated: 2024/09/07 11:37:55 by ukireyeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*add_char(char *str, char c)
{
	char	*res;
	int		len;
	int		clen;

	clen = len = ft_strlen(str);
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

// static char	*capvar(char *str, t_env *env)
// {
// 	int		varlen;
// 	char	*varname;
// 	char	*res;

// 	varlen = 0;
// 	varname = ft_strdup(str);
// 	if (!varname)
// 		return (NULL);
// 	while (str[varlen] && str[varlen] != ' ')
// 		++varlen;
// 	varname[varlen] = '\0';
// 	res = get_var(varname, env);
// 	free(varname);
// 	return (res);
// }

static int	validchar(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static void	skipvar(char **str)
{
	while (**str && validchar(**str))
		++(*str);
}

static char	*get_var(char *str, t_env *env)
{
	char	*var_name;
	int		i;

	var_name = ft_strdup("");
	if (!var_name)
		return (NULL);
	while (*str && validchar(*str))
	{
		var_name = add_char(var_name, *str);
		++str;
	}
	i = find_var_env(env, var_name);
	if (i == -1)
		return (NULL);
	return (env->parsed_env[i][1]);
}

char	*expand_quotes(char *str, t_env *env)
{
	char *res;
	int dquote;
	int squote;
	char *varname;

	squote = dquote = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);

	while (*str)
	{
		if (*str == '"' && !squote)
			dquote = !dquote;
		else if (*str == '\'' && !dquote)
			squote = !squote;
		else if (*str == '$' && !squote)
		{
			++str;
			varname = get_var(str, env);
			if (!varname)
			{
				skipvar(&str);
				continue ;
			}
			res = ft_strjoin(res, varname);
			skipvar(&str);
			continue ;
		}
		else
			res = add_char(res, *str);
		++str;
	}
	return (res);
}