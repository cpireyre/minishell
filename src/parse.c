/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:57:35 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/04 13:25:08 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arena.h"
#include "get_next_line.h"
#include "tokenize.h"
#include "ast.h"
#include "libft.h"
#include <stdio.h>
#include <string.h>

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
	i++;
	return (i);
}

static	char	*get_ast_type(enum e_ast_type t)
{
	if (t == AST_LOGICAL)
		return("AST_LOGICAL");
	else if (t == AST_PIPELINE)
		return("AST_PIPELINE");
	else if (t == AST_COMMAND)
		return("AST_COMMAND");
	else if (t == AST_WORD)
		return("AST_WORD");
	else if (t == AST_REDIR)
		return("AST_REDIR");
	return (NULL);
}

static	void print_ast(t_ast_node *root, size_t level)
{
	size_t i;

	i = 0;
	char *padding = ft_calloc(level * 2 + 1, sizeof(char));
	ft_memset(padding, ' ', level * 2);
	char *value = ft_calloc(root->token.size + 1, sizeof(char));
	ft_memcpy(value, root->token.value, root->token.size);
	ft_printf("%s%s [%s (%s)]\n", padding, get_ast_type(root->type), ast_show_type(root->token.type), value);
	free(value);
	free(padding);
	level++;
	while (i < root->n_children)
	{
		print_ast(root->children[i], level);
		i++;
	}
}

void	expand(t_ast_node *ast, t_arena arena, t_list *env);

void	parse(t_arena arena, t_token *xs, t_list *env)
{
	/* t_token	*token; */
	t_ast_node*	ast;

	size_t	range[2] = {0, 0};
	range[1] = count_toks(xs) - 1; // remove end token
	ast = NULL;
	ast = create_ast(xs, ast, range, arena);
	if (ast)
	{
		print_ast(ast, 0);
		expand(ast, arena, env);
	}
	/* token = xs; */
	// while (token->type != TOK_END)
	// {
	// 	t_ast	*node = new_node_from_token(arena, *token);
	// 	if (node)
	// 	{
	// 		//ft_printf("%s [%s]\n", node->value, ast_show_type(token->type));
	// 	}
	// 	token++;
	// }
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

