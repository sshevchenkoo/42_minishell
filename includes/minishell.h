/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ukireyeu <ukireyeu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:05:57 by yashevch          #+#    #+#             */
/*   Updated: 2024/09/11 20:13:49 by ukireyeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../ft_lib/ft_printf.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

extern int			g_var_thing;

typedef enum s_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	ENV_VAR,
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*content;
	struct s_token	*next;
}					t_token;

typedef struct s_tree
{
	char			**content;
	t_token_type	type;
	int				fd;
	struct s_tree	*left;
	struct s_tree	*right;
}					t_tree;

typedef struct s_env
{
	char			**env;
	char			***parsed_env;
}					t_env;

void				count_quotes(const char *l, int *s, int *d);
void				update_quotes(char a, int *s, int *d);
int					is_invl(const char **line);
int					check_syntax(const char *line);
t_token				*check_and_tokenize(char *line);
void				add_token(t_token **tokens_list, t_token *new_node);
t_token				*new_token(t_token_type type, char *content);
t_tree				*parse_tokens(t_token **tokens);
t_tree				*tree_node(t_token_type type);
int					count_content(t_token *tokens);
t_tree				*file_node(t_token *token);
int					init_shell_env(t_env *env, char **orig_env);
void				free_tree(t_tree *node);
void				update_env(t_env *env, int value, char *var);
void				replace_env_var(char *var, t_env *env);
void				ft_strcopy(char *s1, char *s2, int start, int end);
int					ft_lentchr(char *str, char end);
int					find_var_env(t_env *env, char *str);
void				free_env_var(char ***src);
int					ft_count_digit(int digit);
void				remove_env_var(t_env *env, int i);
char				***dup_env_structure(t_env *env, int c, int n, int i);
char				**copy_env_exclude_index(char **src, int c, int n, int i);
void				free_array(char **src);
char				*get_current_pwd(int size, int try_it, int fd);
char				**unset_or_export(char **cmd, t_env *env, int fd, int *i);
char				**export_cmd(char **cmd, t_env *env, int fd, int **i);
int					env_or_pwd(char *cmd, t_env *env, int fd);
void				set_new_pwd(char *pwd, t_env *env);
int					check_export(char *cmd);
int					export_or_print(char **cmd);
void				append_env(char *src, t_env *env);
void				cleanup_shell(t_env *env, int status);
int					is_valid_echo_param(char *s);
int					echo_cmd(char **cmd, int out_fd);
int					cd_cmd(char **cmd, t_env *env, int fd);
int					change_current_directory(char *path, t_env *env);
int					is_string_numeric(char *str);
void				builtin_exit(char **cmd);
char				*str_without_char(char *str, char del);
void				print_fd_to_stdout(int fd);
void				print_string_to_fd(const char *str, int fd);
int					arr2d_len(char **arr);
char				**arr2d_dup(char **arr);
char				*get_filename(char *path);
char				*get_path(char *cmd, char **envp);
void				traverse_and_execute(t_tree *node, t_env *env, int input_fd,
						int *stat);
// void    print_env(t_env *env, int fd);
char				*expand_quotes(char *str, t_env *env);
void				setup_signal(void);
void				child_ctrl_c(int sig_num);
void				handle_ctrl_c(int a);
void				print_parsed_env(t_env *env);
int					validchar(char c);
void				skipvar(char **str);
void				handle_heredoc_input(char **heredoc_input, t_tree *node);
void				close_and_exec_heredoc(int pipefd[2], t_tree *node,
						t_env *env, int *stat);
void				create_pipe_or_exit(int pipefd[2]);
void				handle_heredoc(t_tree *node, t_env *env, int *stat);
void				traverse_and_execute(t_tree *node, t_env *env, int input_fd,
						int *stat);
void				handle_redirection(t_tree *node, t_env *env, int input_fd,
						int *stat);
void				handle_pipe(t_tree *node, t_env *env, int input_fd,
						int *stat);
void				create_pipe(int pipefd[2]);
void				close_and_exec_pipe(t_tree *node, t_env *env, int input_fd,
						int *stat);
void				handle_fork_exec(t_tree *node, t_env *env, int input_fd,
						int *stat);
void				handle_builtin(t_tree *node, t_env *env, int *stat);
void				quote_handle(char **content, t_env *env);
int					check_builtin(char *str);
void				basic_exec(t_tree *node, t_env *env);
// void	print_orig_env(t_env *env);
// void print_tree(t_tree *root, int level);
// void print_tokens(t_token *tokens_list);

#endif
