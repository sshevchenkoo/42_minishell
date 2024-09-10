#include "../includes/minishell.h"

int					g_var_thing;

void	setup_signal(void)
{
	signal(SIGINT, handle_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}

void	child_ctrl_c(int sig_num)
{
	if (sig_num == 2)
		g_var_thing = 130;
	else
	{
		ft_putstr_fd("Quit", 1);
		g_var_thing = 131;
	}
	write(1, "\n", 1);
}

void	handle_ctrl_c(int a)
{
	(void)a;
	g_var_thing = 1;
	write(1, "\33[2K\r", 5);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}
