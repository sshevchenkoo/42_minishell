#include "../includes/minishell.h"

int	echo_cmd(char **cmd, int out_fd)
{
	int				i;
	int				op_n;

	op_n = 0;
	if (cmd[0] && cmd[1] && is_valid_echo_param(cmd[1]))
		op_n = 1;
	i = op_n + 1;
	while (op_n && cmd[i] && is_valid_echo_param(cmd[i]))
		i++;
	if ((cmd[0] && cmd[i]) || ft_strlen(cmd[i]))
	{
		while (1)
		{
			ft_putstr_fd(cmd[i], out_fd);
			i++;
			if (cmd[i])
				write(out_fd, " ", 1);
			else
				break ;
		}
	}
	if (!op_n)
		write(out_fd, "\n", 1);
	return (0);
}

int	env_or_pwd(char *cmd, t_env *env, int i, int fd)
{
	int					a;
	char				*pwd;

	a = -1;
	if (ft_strncmp(cmd, "env", 3))
	{
		if (i)
			print_export_to_fd(env, fd);
		else
		{
			while (env->parsed_env[++a])
				env_var_fd(env->parsed_env[a][0], env->parsed_env[a][1], fd);
		}
		return (0);
	}
	pwd = get_current_pwd(100, 1, fd);
	if (pwd)
	{
		ft_putendl_fd(pwd, fd);
		return (free(pwd), 0);
	}
	return (256);
}

char    **export_cmd(char **cmd, t_env *env, int fd, int **i)
{
    int d;
    int m;

    d = 1;
    while (cmd[d])
    {
        m = check_export(cmd[d]);
        if (m > 0)
        {
            if (m >= 1 && cmd[d][m] == '+')
                append_env(cmd[d], env);
            else
                replace_env_var(cmd[d], env);
        }
        else
        {
            ft_putendl_fd("Invalid thing", fd);
            **i = 256;
        }
        d++;
    }
    return (cmd);
}

char    **unset_or_export(char **cmd, t_env *env, int fd, int *i)
{
    int a;
    int b;

    a = 1;
    *i = 0;
    if (cmd[a] && ft_strncmp(cmd[0], "unset", 5) == 0)
    {
        while (cmd[a])
        {
            b = find_var_env(env, cmd[a]);
            if (b >= 0)
                remove_env_var(env, b);
            else
                *i = 256;
            a++;
        }
    }
    else if (ft_strncmp(cmd[0], "export", 6) == 0)
    {
        if (export_or_print(cmd))
            cmd = export_cmd(cmd, env, fd, &i);
        else
            env_or_pwd("env", env, 1, fd);
    }
    return (cmd);
}

int	cd_cmd(char **cmd, t_env *env, int fd)
{
	int					a;
	char				*new_path;

	if (cmd[1] && cmd[2])
		ft_putendl_fd("Not a cd thing", fd);
	else
	{
		if (change_current_directory(cmd[1], env) < 0)
			ft_putendl_fd("Only EXISTING destinations", fd);
		else
		{
			a = find_var_env(env, "PWD");
			if (a >= 0)
				remove_env_var(env, a);
			new_path = get_current_pwd(100, 1, fd);
			if (new_path)
			{
				set_new_pwd(new_path, env);
				free(new_path);
			}
			return (0);
		}
	}
	return (256);
}