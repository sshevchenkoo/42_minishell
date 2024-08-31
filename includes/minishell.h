/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yashevch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:05:57 by yashevch          #+#    #+#             */
/*   Updated: 2024/07/03 14:08:30 by yashevch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
# include "../ft_lib/ft_printf.h"

typedef enum	s_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	ENV_VAR,
}	t_token_type;

typedef	struct s_token
{
	t_token_type	type;
	char	*content;
	struct s_token	*next;
}	t_token;

typedef struct s_tree
{
	char	**content;
	t_token_type	type;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

typedef struct	s_env
{
	char	**env;
	char	***parsed_env;
}	t_env;

void	count_quotes(const char *l, int *s, int *d);
void	update_quotes(char a, int *s, int *d);
int	is_invl(const char **line);
int	check_syntax(const char *line);
t_token	*check_and_tokenize(char *line);
void	add_token(t_token **tokens_list, t_token *new);
t_token	*new_token(t_token_type type, char *content);
t_tree	*parse_tokens(t_token **tokens);
t_tree	*tree_node(t_token_type type);
int	count_content(t_token *tokens);
t_tree	*file_node(t_token *token);
int	init_shell_env(t_env *env, char **orig_env);
void	free_tree(t_tree *node);
void    update_env(t_env *env, int value, char *var);
void    replace_env_var(char *var, t_env *env);
void	ft_strcopy(char *s1, char *s2, int start, int end);
int	ft_lentchr(char *str, char end);
int	find_var_env(t_env *env, char *str);
void    free_env_var(char ***src);
int ft_count_digit(int digit);
void    remove_env_var(t_env *env, int i);
char    ***dup_env_structure(t_env *env, int c, int n, int i);
char    **copy_env_exclude_index(char **src, int c, int n, int i);
void    free_array(char **src);
char    *get_current_pwd(int size, int try, int fd);
char    **unset_or_export(char **cmd, t_env *env, int fd, int *i);
char    **export_cmd(char **cmd, t_env *env, int fd, int **i);
int	env_or_pwd(char *cmd, t_env *env, int i, int fd);
void    set_new_pwd(char *pwd, t_env *env);
char    *get_current_pwd(int size, int try, int fd);
int	check_export(char *cmd);
int export_or_print(char **cmd);
void    print_export_to_fd(t_env *env, int fd);
char    ***sort_env(char ***src, int i);
int	str_compare(char *s1, char *s2);
void	print_export_vars(char ***arr, int i, int fd);
void	env_var_fd(char *s1, char *s2, int fd);
void    append_env(char *src, t_env *env);
void	cleanup_shell(t_env *env, int status);
int is_valid_echo_param(char *s);
int	echo_cmd(char **cmd, int out_fd);
int	cd_cmd(char **cmd, t_env *env, int fd);
int	change_current_directory(char *path, t_env *env);
int	is_string_numeric(char *str);
void	builtin_exit(char **cmd);
void	print_parsed_env(t_env *env);
//void print_tree(t_tree *root, int level);
//void print_tokens(t_token *tokens_list);

#endif
