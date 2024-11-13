/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_get_highest_ast_type.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:43:55 by pleander          #+#    #+#             */
/*   Updated: 2024/11/13 10:50:00 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

static void	update_max_ast(enum e_ast_type *max_ast, t_token x)
{
	if ((x.type == TOK_LOGICAL_OR || x.type == TOK_LOGICAL_AND)
		&& (int)AST_LOGICAL < (int)*max_ast)
		*max_ast = AST_LOGICAL;
	else if (x.type == TOK_OPEN_PAREN
		&& ((int)*max_ast > (int)AST_PAREN))
		*max_ast = AST_PAREN;
	else if (x.type == TOK_PIPE
		&& ((int)*max_ast > (int)AST_PIPELINE))
		*max_ast = AST_PIPELINE;
	else if (x.type == TOK_WORD
		&& ((int)*max_ast > (int)AST_COMMAND))
		*max_ast = AST_COMMAND;
}

enum e_ast_type	get_highest_ast_type(t_token *xs, size_t range[2])
{
	enum e_ast_type	max_ast;
	size_t			i;
	int				open_paren;

	open_paren = 0;
	i = range[0];
	max_ast = AST_NONE;
	while (i < range[1])
	{
		if (open_paren == 0)
			update_max_ast(&max_ast, xs[i]);
		if (is_parenthesis(xs[i]))
		{
			if (xs[i].type == TOK_OPEN_PAREN)
				open_paren++;
			else if (xs[i].type == TOK_CLOSE_PAREN)
				open_paren--;
		}
		i++;
	}
	return (max_ast);
}
