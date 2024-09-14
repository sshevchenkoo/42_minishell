/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ukireyeu <ukireyeu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:05:44 by ukireyeu          #+#    #+#             */
/*   Updated: 2024/09/12 17:05:57 by ukireyeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	arr2d_len(char **arr)
{
	int	len;

	len = 0;
	while (arr[len])
		++len;
	return (len);
}

char	**arr2d_dup(char **arr)
{
	int		i;
	int		len;
	char	**new_arr;

	len = arr2d_len(arr);
	i = -1;
	new_arr = malloc(sizeof(char *) * (len + 1));
	if (!new_arr)
		return (NULL);
	while (arr[++i])
		new_arr[i] = ft_strdup(arr[i]);
	new_arr[i] = NULL;
	return (new_arr);
}

char	*get_filename(char *path)
{
	int		len;
	char	*filename;

	if (!path)
		return (NULL);
	len = ft_strlen(path);
	--len;
	while (len > 0 && path[len] != '/')
		--len;
	filename = ft_strdup(path + len + 1);
	if (!filename)
		return (NULL);
	return (filename);
}
