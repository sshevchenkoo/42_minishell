#include "../includes/minishell.h"

void    print_export_to_fd(t_env *env, int *fd)
{
    int i;
    char    ***src;

    i = 0;
    while (env->parsed_env[i])
        i++;
    if (!i)
        return ;
    src = dup_env_structure(env, i, 'X', -1);
    src[i] = 0;
    src = sort_env(src, i);
    print_export_vars(src, i, fd[1]);
    free_env_var(src);
}

int	env_or_pwd(char *cmd, t_env *env, int i, int *fd)
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
				print_env_var_to_fd(env->parsed_env[a][0], env->parsed_env[a][1], fd[1]);
		}
		return (0);
	}
	pwd = get_current_pwd(100, 1, fd[1]);
	if (pwd)
	{
		ft_putendl_fd(pwd, fd[1]);
		return (free(pwd), 0);
	}
	return (256);
}

char    **export_cmd(char **cmd, t_env *env, int *fd, int **i)
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
            ft_putendl_fd("Invalid thing", fd[1]);
            **i = 256;
        }
        d++;
    }
    return (cmd);
}

char    **unset_or_export(char **cmd, t_env *env, int *fd, int *i)
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