/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logicals.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 09:19:39 by pleander          #+#    #+#             */
/*   Updated: 2024/11/06 12:08:12 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arena.h"
#include "ast.h"
#include "execute.h"
#include "tokenize.h"

int	execute_logicals(t_ast_node *ast, t_list *env, t_arena arena)
{
	t_ast_node	*cur;
	int	s;
	
	cur = ast;
	s = execute_ast(cur->children[0], env, arena);
	while (cur->type == AST_LOGICAL)
	{
		if (cur->token.type == TOK_LOGICAL_AND)
		{
			if (cur->children[1]->type == AST_LOGICAL)
				s = !((s == 0) && (execute_ast(cur->children[1]->children[0], env, arena) == 0));
			else
				s = !((s == 0) && (execute_ast(cur->children[1], env, arena) == 0));
		}
		if (cur->token.type == TOK_LOGICAL_OR)
		{
			if (cur->children[1]->type == AST_LOGICAL)
				s = !((s == 0) || (execute_ast(cur->children[1]->children[0], env, arena) == 0));
			else
				s = !((s == 0) || (execute_ast(cur->children[1], env, arena) == 0));
		}
		if (s != 0)
			break ;
		cur = cur->children[1];
	}
	return (s);
}
