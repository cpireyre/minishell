/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:57:35 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/01 09:46:49 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenize.h"

int	has_error_token(t_token *xs)
{
	size_t	i;

	i = 0;
	while (xs[i].type != AST_END)
	{
		if (xs[i].type == AST_ERROR)
			return (1);
		i++;
	}
	return (0);
}

t_ast	*new_node_from_token(t_arena arena, t_token token);

void	parse(t_token *xs)
{
	t_arena	arena;

	arena = arena_new();
	for (t_token *token = xs; token->type != AST_END; token++)
	{
		t_ast	*node = new_node_from_token(arena, *token);
		if (node)
		{
			ft_printf("%s [%s]\n", node->value, ast_show_type(token->type));
		}
	}
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
		if (!result)
			return (NULL);
		ft_memcpy(result->value, token.value, token.size);
	}
	return (result);
}
