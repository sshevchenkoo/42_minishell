/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yashevch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 19:57:50 by yashevch          #+#    #+#             */
/*   Updated: 2024/07/21 19:58:39 by yashevch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/minishell.h"

t_tree	*tree_node(t_token_type type)
{
	t_tree	*node;

	node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	node->type = type;
	node->content = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

int	count_content(t_token *tokens)
{
	int	i;
	int	number;

	number = 0;
	while (tokens && tokens->type == WORD)
	{
		number++;
		tokens = tokens->next;
	}
	return (number);
}

t_tree	*file_node(t_token *token)
{
	t_tree	*node;

	node = tree_node(token->type);
	node->content = malloc(sizeof(char *) * 2);
	if (!node->content)
	{
		free(node);
		return (NULL);
	}
	node->content[0] = token->content;
	node->content[1] = NULL;
	free(token);
	return (node);
}
