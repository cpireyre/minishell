/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:57:35 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/11 10:20:08 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ast.h"
#include "glob.h"
#include "expand.h"

static char	*get_ast_type(enum e_ast_type t);

t_ast_node	*parse(t_arena arena, char *user_input_line, t_list *env)
{
	t_token		*xs;
	t_ast_node	*ast;
	size_t		range[2];

	ast = NULL;
	xs = tokenize(arena, user_input_line);
	if (xs)
	{
		ft_bzero(range, sizeof(range));
		range[1] = count_toks(xs) - 1; // remove end token
		ast = create_ast(xs, ast, range, arena);
		expand(ast, arena, env);
		glob(ast);
	}
	return (ast);
}

/* t_ast	*new_node_from_token(t_arena arena, t_token token) */
/* { */
/* 	t_ast	*result; */

/* 	result = arena_calloc(arena, 1, sizeof(*result)); */
/* 	if (result) */
/* 	{ */
/* 		result->type = token.type; */
/* 		result->value = arena_calloc(arena, 1, token.size + 1); */
/* 		if (!result->value) */
/* 			return (NULL); */
/* 		ft_memcpy(result->value, token.value, token.size); */
/* 		result->n_children = 0; */
/* 		result->children = NULL; */
/* 	} */
/* 	return (result); */
/* } */

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

void	print_ast(t_ast_node *root, size_t level)
{
	size_t	i;
	size_t	pad;

	if (!root)
		return ;
	pad = 0;
	while (pad++ < level * 2)
		ft_printf(" ");
	ft_printf("%s ", get_ast_type(root->type));
	ft_printf("[%s (", ast_show_type(root->token.type));
	i = 0;
	ft_printf("%s", root->token.value);
	ft_printf(")]\n");
	level++;
	i = 0;
	while (i < root->n_children)
		print_ast(root->children[i++], level);
}

static char	*get_ast_type(enum e_ast_type t)
{
	if (t == AST_LOGICAL)
		return ("AST_LOGICAL");
	else if (t == AST_PIPELINE)
		return ("AST_PIPELINE");
	else if (t == AST_COMMAND)
		return ("AST_COMMAND");
	else if (t == AST_WORD)
		return ("AST_WORD");
	else if (t == AST_REDIR)
		return ("AST_REDIR");
	return (NULL);
}
