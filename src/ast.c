/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:07:01 by pleander          #+#    #+#             */
/*   Updated: 2024/10/01 17:04:40 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "arena.h"
#include "get_next_line.h"
#include "libft.h"
#include "tokenize.h"

// typedef struct	s_ast_root
// {
// 	enum e_ast_type	type;
// 	void			*node;
// }	t_ast_node;
/**
 * @brief Calculates the amount of tokens of type in the token array
 *
 * @param xs token array
 * @param type type of token
 * @return amount of tokens of type
 */
static size_t	count_tokens(t_token *xs, enum e_tok_type type, size_t range[2])
{
	size_t	i;
	size_t	n;

	n = 0;
	i = range[0];
	while (i < range[1])
	{
		if (xs[i].type == type)
			n++;
		if (xs[i].type == TOK_END)
			break ;
		i++;
	}
	return (n);
}

static size_t	count_logicals(t_token *xs, size_t range[2])
{
	size_t	tokens;

	tokens = 0;
	tokens += count_tokens(xs, TOK_LOGICAL_AND, range);
	tokens += count_tokens(xs, TOK_LOGICAL_OR, range);
	return (tokens);
}

static enum e_ast_type get_highest_ast_type(t_token *xs, size_t range[2])
{
	if (count_tokens(xs, TOK_LOGICAL_OR, range) +
		count_tokens(xs, TOK_LOGICAL_AND, range) > 0)
		return (AST_LOGICAL);
	else if (count_tokens(xs, TOK_PIPE, range) > 0)
		return (AST_PIPELINE);
	else 
		return (AST_COMMAND);
}

static int	find_next_token_of_type(t_token *xs, enum e_tok_type type, size_t range[2])
{
	size_t	i;
		
	i = range[0];
	while (i < range[1])
	{
		if (xs[i].type == type)
			return (i);
		i++;
		if (xs[i].type == TOK_END)
			break ;
	}
	return (-1);
}

static int	find_next_logical_token(t_token *xs, size_t range[2])
{
	int	or_ind;
	int	and_ind;

	or_ind = find_next_token_of_type(xs, TOK_LOGICAL_OR, range);
	and_ind = find_next_token_of_type(xs, TOK_LOGICAL_AND, range);
	if (or_ind < 0 && and_ind < 0)
		return (find_next_token_of_type(xs, TOK_END, range));
	if (or_ind < 0 && and_ind >= 0)
		return (and_ind);
	if (and_ind < 0 && or_ind >= 0)
		return (or_ind);
	if (and_ind > or_ind)
		return (and_ind);
	else
		return (or_ind);
}

static size_t	count_redirs(t_token *xs, size_t range[2])
{
	size_t	sum;

	sum = 0;
	sum += count_tokens(xs, TOK_REDIRECT_IN, range);
	sum += count_tokens(xs, TOK_REDIRECT_OUT, range);
	sum += count_tokens(xs, TOK_HEREDOC, range);
	return (sum);
}

static void show_token_range(t_token *xs, size_t range[2])
{
	size_t	i;

	i = range[0];
	while (i < range[1])
	{
		tokenize_show(xs[i]);
		i++;
	}
	ft_printf("\n");
}

int	add_node_to_parent(t_ast_node *parent, t_ast_node *child, t_arena arena)
{
	t_ast_node	**children;

	if (!parent)
		return (0);
	if (!child)
		return (-1);
	children = arena_alloc(arena, (parent->n_children + 1) * sizeof(t_ast_node *));
	if (!children)
		return (-1);
	if (parent->children)
		ft_memcpy(children, parent->children, parent->n_children * sizeof(t_ast_node *));
	children[parent->n_children] = child;
	parent->children = children;
	parent->n_children++;
	return (0);
}

static	char	*concat_token_values(t_token *xs, size_t range[2], t_arena arena)
{
	char	*str;
	size_t	len;
	size_t	i;

	len = 0;
	i = range[0];
	while (i < range[1])
	{
		len += xs[i].size + 1;
		i++;
	}
	str = arena_calloc(arena, len, sizeof(char));
	if (!str)
		return (NULL);
	i = range[0];
	while (i < range[1])
	{
		ft_strlcat(str, xs[i].value, len * sizeof(char));
		if (i + 1 < range[1])
			ft_strlcat(str, " ", len * sizeof(char));
		i++;
	}
	return (str);
}

t_ast_node	*create_ast(t_token *xs, t_ast_node *parent, size_t range[2], t_arena arena)
{
	enum e_ast_type max_type;
	int		ltoken;
	size_t	new_range[2];
	t_ast_node	*node;

	node = NULL;
	// ft_printf("Parsing following token array: ");
	// show_token_range(xs, range);
	max_type = get_highest_ast_type(xs, range);
	if (max_type == AST_LOGICAL)
	{
		ltoken = find_next_logical_token(xs, range);
		if (ltoken < 0)
		{
			ft_printf("ERROR\n");
			return (NULL);
		}
		node = arena_calloc(arena, 1, sizeof(t_ast_node));
		if (!node)
			return (NULL);
		node->token = xs[ltoken];
		node->type = AST_LOGICAL;
		new_range[0] = range[0];
		new_range[1] = (size_t)ltoken;
		range[0] = (size_t)ltoken + 1;
		if (add_node_to_parent(node, create_ast(xs, node, new_range, arena), arena) < 0)
		{
			ft_printf("ERROR\n");
			return (NULL);
		}
		if (add_node_to_parent(node, create_ast(xs, node, range, arena), arena) < 0)
		{
			ft_printf("ERROR\n");
			return (NULL);
		}
	}
	else if (max_type == AST_PIPELINE)
	{
		ltoken = find_next_token_of_type(xs, TOK_PIPE, range);
		if (ltoken < 0)
		{
			ft_printf("ERROR\n");
			return (NULL);
		}
		node = arena_calloc(arena, 1, sizeof(t_ast_node));
		if (!node)
			return (NULL);
		node->token = xs[ltoken];
		node->type = AST_PIPELINE;
		new_range[0] = range[0];
		new_range[1] = (size_t)ltoken;
		range[0] = (size_t)ltoken + 1;
		if (add_node_to_parent(node, create_ast(xs, node, new_range, arena), arena) < 0)
		{
			ft_printf("ERROR\n");
			return (NULL);
		}
		if (add_node_to_parent(node, create_ast(xs, node, range, arena), arena) < 0)
		{
			ft_printf("ERROR\n");
			return (NULL);
		}
	}
	else if (max_type == AST_COMMAND)
	{
		// command has first program name, then arguments. If thers's a redirect the following file is outfile

		node = arena_alloc(arena, sizeof(t_ast_node));
		if (!node)
			return (NULL);
		size_t word_count = count_tokens(xs, TOK_WORD, range);
		node->type = AST_COMMAND;
		node->token.value = concat_token_values(xs, range, arena);
		node->token.size = ft_strlen(node->token.value);
		node->token.type = TOK_COMMAND;
		node->n_children = word_count; // Redirs add a redir but also remove a word
		node->children = arena_calloc(arena, node->n_children, sizeof(t_ast_node *));
		if (!node->children)
		{
			ft_printf("Error\n");
			return (NULL);
		}
		int i = 0;
		while (range[0] + i < range[1] && xs[range[0] + i].type != TOK_END)
		{
			node->children[i] = arena_calloc(arena, 1, sizeof(t_ast_node));
			if (!node->children[i])
			{
				ft_printf("ERROR\n");
				return (NULL);
			}
			node->children[i]->token = xs[range[0] + i];
			if (xs[range[0] + i].type == TOK_WORD)
				node->children[i]->type = AST_WORD;
			else
			{
				node->children[i]->type = AST_REDIR;
				if (range[0] + i + 1 < range[1] && xs[range[0] + i].type != TOK_END)
				{
					node->children[i]->children = arena_calloc(arena, 1, sizeof(t_ast_node *));
					if (!node->children[i]->children)
						return (NULL);
					t_ast_node *redir_child = arena_calloc(arena, 1, sizeof(t_ast_node));
					redir_child->type = AST_WORD;
					redir_child->token = xs[range[0] + i + 1];
					redir_child->n_children = 0;
					node->children[i]->n_children = 1;
					*node->children[i]->children = redir_child;
					i++;
				}
				else 
				{
					ft_dprintf(2, "Syntax error\n");
					return (NULL);
				}
			}
			i++;
		}
	}
	else
		ft_printf("ERROR\n");
	return (node);
}
