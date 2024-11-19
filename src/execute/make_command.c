/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:09:05 by pleander          #+#    #+#             */
/*   Updated: 2024/11/14 13:15:54 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static int	parse_children(t_command *cmd, t_ast_node *ast, t_list *env,
		t_arena a)
{
	size_t	i;
	size_t	arg_i;

	i = -1;
	arg_i = 0;
	while (++i < ast->n_children)
	{
		if (ast->children[i]->type == AST_WORD)
		{
			if (cmd->path == NULL)
			{
				cmd->path = find_path(ast->children[i]->token.value, env, a);
				if (!cmd->path)
					return (-1);
				cmd->args[arg_i] = (char *)ast->children[i]->token.value;
			}
			else
				cmd->args[arg_i] = (char *)ast->children[i]->token.value;
			arg_i++;
		}
		else if (ast->children[i]->type == AST_REDIR)
			if (handle_redir(cmd, ast->children[i]) < 0)
				return (-1);
	}
	return (0);
}

int	make_command(t_command *cmd, t_ast_node *ast, t_list *env, t_arena arena)
{
	if (ast->type != AST_COMMAND)
	{
		ft_dprintf(2, "Error: Wrong AST type\n");
		return (-1);
	}
	cmd->path = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->args = arena_calloc(arena, count_cmd_args(ast) + 1, sizeof(char *));
	if (!cmd->args)
		return (-1);
	if (parse_children(cmd, ast, env, arena) < 0)
		return (-1);
	return (0);
}
