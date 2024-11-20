/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:09:05 by pleander          #+#    #+#             */
/*   Updated: 2024/11/20 14:26:40 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static void	set_ints(size_t	*i, size_t *arg_i)
{
	*i = -1;
	*arg_i = 0;
}

static int	parse_children(t_command *cmd, t_ast_node *ast, t_list *env,
		t_arena a)
{
	size_t	i;
	size_t	arg_i;
	int		handle_err;

	set_ints(&i, &arg_i);
	while (++i < ast->n_children)
	{
		if (ast->children[i]->type == AST_WORD)
		{
			cmd->args[arg_i++] = (char *)ast->children[i]->token.value;
			if (cmd->path == NULL)
			{
				cmd->path = find_path(ast->children[i]->token.value, env, a);
				if (!cmd->path)
					return (-1);
			}
		}
		else if (ast->children[i]->type == AST_REDIR)
		{
			handle_err = handle_redir(cmd, ast->children[i]);
			if (handle_err < 0)
				return (handle_err);
		}
	}
	return (0);
}

int	make_command(t_command *cmd, t_ast_node *ast, t_list *env, t_arena arena)
{
	int	err;

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
	err = parse_children(cmd, ast, env, arena);
	if (err < 0)
		return (err);
	return (0);
}
