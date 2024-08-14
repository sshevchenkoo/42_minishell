#include "../includes/minishell.h"

int	ft_lentchr(char *str, char end)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\0' && str[i] != end)
		i++;
	return (i);
}

void	ft_strcopy(char *s1, char *s2, int start, int end)
{
	int	a;

	a = 0;
	while (start < end)
	{
		s1[a] = s2[start];
		a++;
		start++;
	}
	s1[a] = '\0';
}

char	**duplicate_env(char **env)
{
	int				a;
	int				b;
	char			**new_old;

	a = 0;
	while (env[a])
		a++;
	new_old = malloc((a + 1) * sizeof(char *));
	if (!new_old)
		return (NULL);
	b = 0;
	while (b < a)
	{
		new_old[b] = ft_strdup(env[b]);
		if (!new_old[b])
		{
			while (b > 0)
				free(new_old[--b]);
			free(new_old);
			return (NULL);
		}
		b++;
	}
	new_old[b] = NULL;
	return (new_old);
}


int	init_shell_struct(t_env *env, char **orig_env, int i, int m)
{
	int	n;

	env->env = duplicate_env(orig_env);
	while (orig_env[i])
		i++;
	env->parsed_env = malloc(sizeof(char **) * (i + 1));
	if (!env->parsed_env)
		return (0);
	while (m < i)
	{
		n = ft_lentchr(orig_env[m], '=');
		env->parsed_env[m] = malloc(2 * sizeof(char *));
		env->parsed_env[m][0] = malloc(n * sizeof(char));
		env->parsed_env[m][1] = malloc((ft_strlen(orig_env[b]) - n) * sizeof(char));
		if (!env->parsed_env[m] || !env->parsed_env[m][0] || !env->parsed_env[m][1])
			return (0);
		ft_strcopy(env->parsed_env[m][0], orig_env[m], 0, n);
		ft_strcopy(env->parsed_env[m][1], orig_env[m], n + 1, ft_strlen(orig_env[b]));
		m++;
	}
	env->parsed_env[m] = NULL;
	return (1);
}

int	init_shell_env(t_env *env, char **orig_env)
{
	int	status;

	status = init_shell_struct(env, orig_env, 0, 0);
	return (status)
}
