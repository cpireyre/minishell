/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:57:35 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/01 17:03:42 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arena.h"
#include "get_next_line.h"
#include "tokenize.h"
#include "ast.h"



int	has_error_token(t_token *xs)
{
	size_t	i;

	i = 0;
	while (xs[i].type != TOK_END)
	{
		if (xs[i].type == TOK_ERROR)
			return (1);
		i++;
	}
	return (0);
}

t_ast	*new_node_from_token(t_arena arena, t_token token);

static size_t	count_toks(t_token *xs)
{
	size_t i;

	i = 0;
	while (xs[i].type != TOK_END)
		i++;
	return (i);
}

void	parse(t_token *xs)
{
	t_arena	arena;
	t_token	*token;
	t_ast_node*	ast;

	arena = arena_new();
	size_t	range[2] = {0, 0};
	range[1] = count_toks(xs);
	ast = NULL;
	create_ast(xs, ast, range, arena);
	token = xs;
	// while (token->type != TOK_END)
	// {
	// 	t_ast	*node = new_node_from_token(arena, *token);
	// 	if (node)
	// 	{
	// 		//ft_printf("%s [%s]\n", node->value, ast_show_type(token->type));
	// 	}
	// 	token++;
	// }
	arena_dispose(&arena);
}


t_ast	*new_node_from_token(t_arena arena, t_token token)
{
	t_ast	*result;

	result = arena_calloc(arena, 1, sizeof(*result));
	if (result)
	{
		result->type = token.type;
		result->value = arena_calloc(arena, 1, token.size + 1);
		if (!result->value)
			return (NULL);
		ft_memcpy(result->value, token.value, token.size);
		result->n_children = 0;
		result->children = NULL;
	}
	return (result);
}

// int	add_child_to_node(t_ast *node, t_ast *child, t_arena arena)
// {
// 	t_ast **new_child_array;
//
// 	new_child_array = arena_calloc(arena, node->n_children + 1, sizeof(t_ast *));
// 	if (!new_child_array)
// 		return (-1);
// 	ft_memcpy(new_child_array, node->children, node->n_children);
// 	node->n_children++;
// 	new_child_array[node->n_children - 1] = child;
// 	return (0);
// }

