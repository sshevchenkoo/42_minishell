/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yashevch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 13:34:42 by yashevch          #+#    #+#             */
/*   Updated: 2024/07/10 13:32:16 by yashevch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/minishell.h"

void	token_chars(char **line, t_token **tokens_list)
{
	if (**line == '>')
	{
		if (*(*line + 1) == '>')
		{
			add_token(tokens_list, new_token(APPEND, ">>"));
			(*line)++;
		}
		else
			add_token(tokens_list, new_token(REDIR_OUT, ">"));
	}
	else if (**line == '<')
	{
		if (*(*line + 1) == '<')
		{
			add_token(tokens_list, new_token(HEREDOC, "<<"));
			(*line)++;
		}
		else
			add_token(tokens_list, new_token(REDIR_IN, "<"));
	}
	else if (**line == '|')
		add_token(tokens_list, new_token(PIPE, "|"));
	(*line)++;
}

void	handle_token_content(char *start, int len, t_token **token_lists)
{
	char	*content;
	int		i;

	content = malloc(sizeof(char) * len + 1);
	if (content)
	{
		content[len] = '\0';
		i = 0;
		while (i < len)
		{
			content[i] = start[i];
			i++;
		}
		add_token(token_lists, new_token(WORD, content));
		free(content);
	}
	else
		ft_putstr_fd("Error: Malloc failed.\n", 2);
}

void	token_words(char **line, t_token **token_lists)
{
	char	quote;
	char	*start;
	int		flag;
	int		len;

	len = 0;
	start = *line;
	flag = 0;
	quote = '\0';
	while (**line)
	{
		if (!flag && (**line == 34 || **line == 39))
		{
			flag = 1;
			quote = **line;
		}
		else if (flag && **line == quote)
			flag = 0;
		if (!flag && ft_strchr(" \t\n><|", **line))
			break ;
		len++;
		(*line)++;
	}
	if (len > 0)
		handle_token_content(start, len, token_lists);
}

t_token	*tokenize_input(char *line)
{
	t_token	*tokens_list;

	tokens_list = NULL;
	while (*line)
	{
		while (*line && (*line == ' ' || *line == '\t'))
			line++;
		if (*line == '|' || *line == '<' || *line == '>')
			token_chars(&line, &tokens_list);
		else
			token_words(&line, &tokens_list);
	}
	return (tokens_list);
}

t_token	*check_and_tokenize(char *line)
{
	t_token	*tokens;
	char	*trim_line;

	trim_line = ft_strtrim(line, " \t\n\v\f\r");
	free(line);
	if (!trim_line)
		return (NULL);
	if (check_syntax(trim_line))
	{
		free(trim_line);
		return (NULL);
	}
	tokens = tokenize_input(trim_line);
	free(trim_line);
	return (tokens);
}

/*void print_token_type(t_token_type type)
{
    if (type == WORD)
        printf("WORD");
    else if (type == APPEND)
        printf("APPEND");
    else if (type == REDIR_OUT)
        printf("REDIR_OUT");
    else if (type == REDIR_IN)
        printf("REDIR_IN");
    else if (type == HEREDOC)
        printf("HEREDOC");
    else if (type == PIPE)
        printf("PIPE");
    else
        printf("UNKNOWN");
}

void print_tokens(t_token *tokens_list)
{
    t_token *current = tokens_list;
    while (current)
    {
        printf("Token Type: ");
        print_token_type(current->type);
        printf(", Content: %s\n", current->content);
        current = current->next;
    }
}*/
