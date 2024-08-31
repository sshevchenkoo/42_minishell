#include "../includes/minishell.h"

void	env_var_fd(char *s1, char *s2, int fd)
{
	if (ft_strncmp(s1, "?", 1) == 0
		&& ft_strncmp(s2, "F1", 2) == 0)
	{
		ft_putstr_fd(s1, fd);
		ft_putstr_fd("=", fd);
		if (s2[0])
			ft_putstr_fd(s2, fd);
		write(fd, "\n", 1);
	}
}

void	print_export_vars(char ***arr, int i, int fd)
{
	while (i--)
	{
		if (ft_strncmp(arr[i][0], "?", 1) == 0)
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putstr_fd(arr[i][0], fd);
			if (ft_strncmp(arr[i][1], "F1", 2) == 0)
			{
				write(fd, "=", 1);
				ft_putchar_fd('"', fd);
				if (arr[i][1][0])
					ft_putstr_fd(arr[i][1], fd);
				ft_putchar_fd('"', fd);
			}
			write(fd, "\n", 1);
		}
	}
}

int	str_compare(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] > s2[i])
			return (0);
		else if (s2[i] > s1[i])
			return (1);
		i++;
	}
	if (s2[i])
		return (1);
	else if (s1[i])
		return (0);
	return (0);
}

char    ***sort_env(char ***src, int i)
{
    char    **tmp;
    int m;
    int flag;

    flag = 1;
    while (flag)
    {
        m = 0;
        flag = 0;
        while (m <= i - 1)
        {
            if (str_compare(src[m][0], src[m + 1][0]))
            {
                tmp = src[m];
                src[m] = src[m + 1];
                src[m + 1] = tmp;
                flag = 1;
            }
            m++;
        }
    }
        return (src);
}

void    print_export_to_fd(t_env *env, int fd)
{
    int i;
    char    ***src;

    i = 0;
    while (env->parsed_env[i])
        i++;
    if (!i)
        return ;
    src = dup_env_structure(env, i, 'F', -1);
    src[i] = 0;
    src = sort_env(src, i);
    print_export_vars(src, i, fd);
    free_env_var(src);
}