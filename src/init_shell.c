#include "../includes/minishell.h"

int	find_var_env(t_env *env, char *str)
{
	int	i;
	int	t;

	i = 0;
	while (env->parsed_env[i])
	{
		t = 0;
		while (env->parsed_env[i][0][t] && str[t]
				&& env->parsed_env[i][0][t] == str[t])
			t++;
		if (t == ft_strlen(str) && t == ft_strlen(env->parsed_env[i][0]))
			return (i);
		i++;
	}
	return (-1);
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
		env->parsed_env[m][0] = malloc(n * sizeof(char *));
		env->parsed_env[m][1] = malloc((ft_strlen(orig_env[m]) - n)
				* sizeof(char *));
		if (!env->parsed_env[m] || !env->parsed_env[m][0]
			|| !env->parsed_env[m][1])
			return (0);
		ft_strcopy(env->parsed_env[m][0], orig_env[m], 0, n);
		ft_strcopy(env->parsed_env[m][1], orig_env[m], n + 1,
			ft_strlen(orig_env[m]));
		m++;
	}
	env->parsed_env[m] = NULL;
	return (1);
}

void	init_default_var(t_env *env, int a)
{
	char	*pwd;

	a = find_var_env(env, "SHELL");
	if (a >= 0)
		remove_env_var(env, a);
	replace_env_var("SHELL=minishell42", env);
	replace_env_var("?=0", env);
	a = find_var_env(env, "PWD");
	pwd = get_current_pwd(100, 1, 2);
	if (pwd)
	{
		if (a >= 0)
			remove_env_var(env, a);
		set_new_pwd(pwd, env);
		free(pwd);
	}
}

int	init_shell_env(t_env *env, char **orig_env)
{
	int	status;
	int	index;
	int	a;

	status = init_shell_struct(env, orig_env, 0, 0);
	a = find_var_env(env, "SHLVL");
	index = ft_atoi(env->parsed_env[a][1]);
	update_env(env, index + 1, "SHLVL=");
	init_default_var(env, 0);
	return (status);
}
