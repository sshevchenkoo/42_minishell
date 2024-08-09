#include "../includes/minishell.h"

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
	new_old[b] = 0;
	return (new_old);
}


int	init_shell_struct(t_env *env, char **orig_env, int i, int m)
{
	env->env = duplicate_env(orig_env);
}

int	init_shell_env(t_env *env, char **orig_env)
{
	int	status;

	status = init_shell_struct(env, orig_env, 0, -1);
	return (status)
}
