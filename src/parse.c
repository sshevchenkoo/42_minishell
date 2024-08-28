/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yashevch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 16:11:04 by yashevch          #+#    #+#             */
/*   Updated: 2024/07/14 20:20:41 by yashevch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/minishell.h"

t_tree	*parse_command(t_token **tokens)
{
	t_tree	*command;
	int		number;
	int		i;
	t_token	*tmp;

	command = tree_node(WORD);
	number = count_content(*tokens);
	command->content = malloc(sizeof(char *) * (number + 1));
	if (!command)
		return (NULL);
	i = 0;
	while (i < number)
	{
		command->content[i] = ft_strdup((*tokens)->content);
		tmp = *tokens;
		*tokens = (*tokens)->next;
		free(tmp->content);
		free(tmp);
		i++;
	}
	command->content[number] = NULL;
	return (command);
}

t_tree	*parse_red2(t_token **tokens)
{
	t_token	*tmp;
	t_tree	*red_node;
	t_token	*next_t;

	tmp = *tokens;
	while (*tokens && (*tokens)->next)
	{
		next_t = (*tokens)->next;
		if ((*tokens)->next->type >= REDIR_IN
			&& (*tokens)->next->type <= HEREDOC)
		{
			red_node = tree_node((*tokens)->next->type);
			(*tokens)->next = next_t->next->next;
			red_node->left = parse_red2(&tmp);
			red_node->right = file_node(next_t->next);
			return (free(next_t->content), free(next_t), red_node);
		}
		*tokens = (*tokens)->next;
	}
	return (parse_command(&tmp));
}

t_tree	*parse_red(t_token **tokens)
{
	t_token	*tmp;
	t_tree	*red_node;

	tmp = *tokens;
	if ((*tokens)->type == REDIR_IN
		|| (*tokens)->type == REDIR_OUT
		|| (*tokens)->type == APPEND
		|| (*tokens)->type == HEREDOC)
	{
		red_node = tree_node((*tokens)->type);
		*tokens = (*tokens)->next->next;
		red_node->left = parse_red(tokens);
		red_node->right = file_node(tmp->next);
		free(tmp->content);
		free(tmp);
		return (red_node);
	}
	return (parse_red2(tokens));
}

t_tree	*parse_tokens(t_token **tokens)
{
	t_tree	*tree;
	t_token	*start;
	t_token	*next;

	start = *tokens;
	while ((*tokens) && (*tokens)->next)
	{
		next = (*tokens)->next;
		if ((*tokens)->next->type == PIPE)
		{
			tree = tree_node(PIPE);
			(*tokens)->next = NULL;
			tree->left = parse_red(&start);
			tree->right = parse_tokens(&(next->next));
			free(next->content);
			free(next);
			return (tree);
		}
		*tokens = (*tokens)->next;
	}
	return (parse_red(&start));
}

/*void print_tree(t_tree *root, int level) {
    if (root == NULL) {
        return;
    }

    // Выводим отступы для текущего уровня
    for (int i = 0; i < level; ++i) {
        printf("    ");
    }

    // Выводим тип узла и его содержимое, если есть
    switch (root->type) {
        case WORD:
            printf("WORD: ");
            for (int i = 0; root->content[i] != NULL; ++i) {
                printf("%s ", root->content[i]);
            }
            printf("\n");
            break;
        case PIPE:
            printf("PIPE\n");
            break;
        case REDIR_IN:
            printf("REDIR_IN\n");
            break;
        case REDIR_OUT:
            printf("REDIR_OUT\n");
            break;
        case APPEND:
            printf("APPEND\n");
            break;
        case HEREDOC:
            printf("HEREDOC\n");
            break;
        default:
            printf("UNKNOWN\n");
            break;
    }

    // Рекурсивно обходим левое поддерево
    if (root->left) {
        print_tree(root->left, level + 1);
    }

    // Рекурсивно обходим правое поддерево
    if (root->right) {
        print_tree(root->right, level + 1);
    }
}
*/
