/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logicals.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 09:19:39 by pleander          #+#    #+#             */
/*   Updated: 2024/11/14 11:54:23 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arena.h"
#include "ast.h"
#include "execute.h"
#include "tokenize.h"

static t_shell_status	execute_logical_branch(t_ast_node *cur, t_list *env,
		t_arena arena, int prev_exit)
{
	if (cur->children[1]->type == AST_LOGICAL)
		return (execute_ast(
				cur->children[1]->children[0], env, arena, prev_exit));
	else
		return (execute_ast(cur->children[1], env, arena, prev_exit));
}

t_shell_status	execute_logicals(
		t_ast_node *ast, t_list *env, t_arena arena, int prev_exit)
{
	t_ast_node		*cur;
	t_shell_status	status;

	cur = ast;
	status = execute_ast(cur->children[0], env, arena, prev_exit);
	while (cur->type == AST_LOGICAL && !status.sigint_received
		&& !status.should_exit)
	{
		if (cur->token.type == TOK_LOGICAL_AND)
		{
			if (status.exit_code == 0)
				status = execute_logical_branch(cur, env, arena, prev_exit);
		}
		else if (cur->token.type == TOK_LOGICAL_OR)
		{
			if (status.exit_code != 0)
				status = execute_logical_branch(cur, env, arena, prev_exit);
		}
		if (status.exit_code != 0)
			break ;
		cur = cur->children[1];
	}
	return (status);
}
