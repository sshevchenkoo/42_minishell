#include "../includes/minishell.h"

int	is_valid_echo_param(char *s)
{
	int	i;

	i = 0;
	if (s && s[i] && s[i] == '-')
	{
		while (s[++i])
		{
			if (s[i] != 'n')
				return (0);
		}
	}
	else
		return (0);
	return (1);
}

int	export_or_print(char **cmd)
{
	int	a;

	a = 1;
	while (cmd[0] && cmd[a])
	{
		if (cmd[a])
			return (1);
		a++;
	}
	return (0);
}

int	check_export(char *cmd)
{
	int	a;
	int	b;

	a = ft_lentchr(cmd, '=');
	if (a > 1 && cmd[a - 1] == '+')
		a -= 1;
	if (a)
	{
		b = 0;
		while (b < a)
		{
			if (!b && !ft_isalpha(cmd[b]) && cmd[b] != '_')
				return (-1);
			else if (b && !ft_isalnum(cmd[b]) && cmd[b] != '_')
				return (-1);
			b++;
		}
	}
	return (a);
}

char	*get_current_pwd(int size, int try, int fd)
{
	char	*buffer;

	buffer = malloc(size);
	if (!buffer)
		return (NULL);
	if (!getcwd(buffer, size))
	{
		free(buffer);
		if (try < 10)
			return (get_current_pwd(size + 50, try + 1, fd));
		else
		{
			ft_putendl_fd("Error get pwd", fd);
			return (NULL);
		}
	}
	return (buffer);
}

void	set_new_pwd(char *pwd, t_env *env)
{
	char **pwd_export;
	int i;
	int m;

	i = 0;
	m = 4;
	pwd_export = malloc(sizeof(char *) * 3);
	pwd_export[0] = ft_strdup("export");
	pwd_export[1] = malloc(ft_strlen(pwd) + 5);
	if (!pwd_export[1])
		return ;
	ft_strcopy(pwd_export[1], "PWD=", 0, 4);
	while (pwd[i])
		pwd_export[1][m++] = pwd[i++];
	pwd_export[1][m] = '\0';
	pwd_export[2] = 0;
	unset_or_export(pwd_export, env, -1, &i);
	free_array(pwd_export);
}