#include "../includes/minishell.h"

int	is_string_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	change_current_directory(char *path, t_env *env)
{
	int					status;
	int					a;

	if (!path || !ft_strlen(path))
	{
		a = find_var_env(env, "HOME");
		if (a >= 0)
			status = chdir(env->parsed_env[a][1]);
		else
			status = -1;
	}
	else
		status = chdir(path);
	return (status);
}

void	builtin_exit(char **cmd)
{
	int	status;

	status = 0;
	if (cmd[1] && cmd[2])
		status = 1;
	else if (cmd[1] && !is_string_numeric(cmd[1]))
		status = 255;
	else if (cmd[1])
		status = ft_atoi(cmd[1]);
	free_array(cmd);
	exit(status);
}
