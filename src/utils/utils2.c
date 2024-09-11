/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ukireyeu <ukireyeu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 17:02:58 by ukireyeu          #+#    #+#             */
/*   Updated: 2024/09/11 12:21:46 by ukireyeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

static char	**find_paths(char **envp)
{
	char	**paths;
	char	*path;

	path = NULL;
	paths = NULL;
	while (envp && *envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
		{
			path = ft_strdup(&(*envp)[5]);
			break ;
		}
		envp++;
	}
	paths = ft_split(path, ':');
	if (path)
		free(path);
	return (paths);
}

char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	*result;
	char	**allpath;
	char	*tmp;

	i = 0;
	allpath = find_paths(envp);
	while (allpath[i])
	{
		tmp = ft_strjoin(allpath[i], "/");
		result = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(result, F_OK | X_OK) == 0)
			return (result);
		free(result);
		i++;
	}
	ft_free_tab(allpath);
	return (NULL);
}

void	print_fd_to_stdout(int fd)
{
	char	buffer[1024];
	ssize_t	bytes_read;

	bytes_read = read(fd, buffer, sizeof(buffer));
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, sizeof(buffer));
		if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read)
		{
			perror("write to stdout failed");
			return ;
		}
	}
	if (bytes_read < 0)
	{
		perror("read from file descriptor failed");
	}
}

void	print_string_to_fd(const char *str, int fd)
{
	int		bytes_written;
	int		length;
	char	eot;

	length = ft_strlen(str);
	bytes_written = write(fd, str, length);
	if (bytes_written != length)
	{
		perror("write to file descriptor failed");
		return ;
	}
	eot = 0x04;
	bytes_written = write(fd, &eot, 1);
	if (bytes_written != 1)
	{
		perror("write of EOT to file descriptor failed");
	}
}
