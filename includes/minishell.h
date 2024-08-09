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
//void print_tree(t_tree *root, int level);
//void print_tokens(t_token *tokens_list);

#endif
