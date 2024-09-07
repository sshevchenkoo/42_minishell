#include "../includes/minishell.h"

int	sizeof_str(char *str, char end)
{
	int			a;

	a = 0;
	while (str && str[a] != '\0' && str[a] != end)
		a += 1;
	return (a);
}

int	str_cmp(char *s_1, char *s_2, char *s_3)
{
	int					a;

	a = 0;
	while (s_1[a] && s_2[a] && s_1[a] == s_2[a])
		a += 1;
	if (a == sizeof_str(s_1, '\0')
		&& a == sizeof_str(s_2, '\0'))
		return (1);
	if (s_3)
		return (str_cmp(s_1, s_3, NULL));
	return (0);
}

void	print_export_vars_1(char ***arr, int a, int fd)
{
	while (a--)
	{
		if (!str_cmp(arr[a][0], "?", NULL))
		{
			ft_putstr_fd(arr[a][0], fd);
			if (!str_cmp(arr[a][1], "F1", NULL))
			{
				write(fd, "=", 1);
				if (arr[a][1][0])
					ft_putstr_fd(arr[a][1], fd);
			}
			write(fd, "\n", 1);
		}
	}
}

void    print_env(t_env *env, int fd)
{
	char				***new_array;
	int					a;

	a = 0;
	while (env->parsed_env[a] != 0)
		a++;
	if (!a)
		return ;
	new_array = dup_env_structure(env, a, 'F', -1);
	new_array[a] = 0;
	print_export_vars_1(new_array, a, fd);
	free_env_var(new_array);
}

/*void	print_parsed_env(t_env *env)
{
    int i = 0;

    while (env->parsed_env[i] != NULL)
    {
        printf("%s=", env->parsed_env[i][0]);
        printf("%s\n", env->parsed_env[i][1]);
        i++;
    }
}

void	print_orig_env(t_env *env)
{
    int i = 0;

    while (env->env[i] != NULL)
    {
        printf("%s\n", env->env[i]);
        i++;
    }
}*/