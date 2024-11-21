/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:07:01 by pleander          #+#    #+#             */
/*   Updated: 2024/11/14 14:46:27 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "arena.h"
#include "get_next_line.h"
#include "libft.h"
#include "tokenize.h"

int	add_node_to_parent(t_ast_node *parent, t_ast_node *child, t_arena arena)
{
	t_ast_node	**children;

	if (!parent)
		return (0);
	if (!child)
		return (-1);
	children = arena_alloc(arena,
			(parent->n_children + 1) * sizeof(t_ast_node *));
	if (!children)
		return (-1);
	if (parent->children)
		ft_memcpy(children,
			parent->children, parent->n_children * sizeof(t_ast_node *));
	children[parent->n_children] = child;
	parent->children = children;
	parent->n_children++;
	return (0);
}

static t_ast_node	*create_ast_logical(t_token *xs, size_t range[2],
		t_arena arena)
{
	int			ltoken;
	size_t		new_range[2];
	t_ast_node	*node;

	ltoken = find_next_logical_token(xs, range);
	node = arena_calloc(arena, 1, sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->token = xs[ltoken];
	node->type = AST_LOGICAL;
	new_range[0] = range[0];
	new_range[1] = (size_t)ltoken;
	range[0] = (size_t)ltoken + 1;
	if (range[1] - range[0] < 1 || new_range[1] - new_range[0] < 1)
	{
		syntax_error();
		return (NULL);
	}
	if (add_node_to_parent(node, create_ast(xs, new_range, arena), arena) < 0)
		return (NULL);
	if (add_node_to_parent(node, create_ast(xs, range, arena), arena) < 0)
		return (NULL);
	return (node);
}

/**
 * @brief Creates a parenthesis node in the AST tree.
 * basically removes parentheses and parses section to it's own branch.
 *
 * @param xs: token array
 * @param range: token array range
 * @param arena: the memory arena
 * @return: new ast node
 */
static t_ast_node	*create_ast_paren(t_token *xs, size_t range[2],
		t_arena arena)
{
	t_ast_node	*node;

	node = arena_calloc(arena, 1, sizeof(t_ast_node));
	if (!node)
		return (NULL);
	if (!is_logical_token(xs[range[0] - 1]))
	{
		syntax_error();
		return (NULL);
	}
	node->token = xs[range[0]];
	node->type = AST_PAREN;
	range[0]++;
	range[1]--;
	if (add_node_to_parent(node, create_ast(xs, range, arena), arena) < 0)
		return (NULL);
	return (node);
}

static t_ast_node	*create_ast_pipe(t_token *xs, size_t range[2],
		t_arena arena)
{
	int			ltoken;
	size_t		new_range[2];
	t_ast_node	*node;

	ltoken = find_next_token_of_type(xs, TOK_PIPE, range);
	node = arena_calloc(arena, 1, sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->token = xs[ltoken];
	node->type = AST_PIPELINE;
	new_range[0] = range[0];
	new_range[1] = (size_t)ltoken;
	range[0] = (size_t)ltoken + 1;
	if (add_node_to_parent(node, create_ast(xs, new_range, arena), arena) < 0)
		return (NULL);
	if (add_node_to_parent(node, create_ast(xs, range, arena), arena) < 0)
		return (NULL);
	return (node);
}

t_ast_node	*create_ast(t_token *xs, size_t range[2], t_arena arena)
{
	enum e_ast_type	max_type;
	t_ast_node		*node;

	max_type = get_highest_ast_type(xs, range);
	if (max_type == AST_LOGICAL)
		node = create_ast_logical(xs, range, arena);
	else if (max_type == AST_PAREN)
		node = create_ast_paren(xs, range, arena);
	else if (max_type == AST_PIPELINE)
		node = create_ast_pipe(xs, range, arena);
	else if (max_type == AST_COMMAND)
		node = create_command_node(xs, range, arena);
	else
		return (NULL);
	return (node);
}
