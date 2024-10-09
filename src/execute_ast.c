/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:51:52 by pleander          #+#    #+#             */
/*   Updated: 2024/10/08 20:56:02 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arena.h"
#include "libft.h"
#include "ast.h"
#include "minishell.h"

/**
 * @brief Executes the AST and returns the exit code
 *
 * @param ast AST node
 * @return exit code
 */
int	execute_ast(t_ast_node *ast)
{
	int	cur_child;
	int i;

	if (ast->type == AST_COMMAND)
	{
		i = 0;
		ft_printf("Execute AST command: ");
		while (i < ast->token.size)
			ft_printf("%c", ast->token.value[i++]);
		ft_printf("\n");
		return (0);
	}

	cur_child = 0;
	while (cur_child < ast->n_children)
	{
		if (ast->type == AST_PIPELINE)
		{
			ft_printf("Execute pipeline\n");
			execute_ast(ast->children[cur_child]);
		}
		else if (ast->type == AST_LOGICAL)
		{
			ft_printf("Execute logical statment\n");
			execute_ast(ast->children[cur_child]);
		}
		else
			ft_printf("We should not be here..\n");
		cur_child++;
	}
	return (0);
}

int		count_cmd_args(t_ast_node *ast)
{
	int c;
	int i;

	c = 0;
	i = 0;
	while (i < ast->n_children)
	{
		if (ast->children[i]->type == AST_WORD)
			c++;
		i++;
	}
	return (c);
}

int	parse_command(t_command *cmd, t_ast_node *ast, t_arena arena)
{
	int	i;

	i = 0;
	cmd->args = arena_calloc(arena, count_cmd_args(ast), sizeof(char *));
	if (cmd->args)
		return (-1);
	// TODO: copy commands to cmd
	// Open file descriptors and store them in cmd
	// Read heredoc
	return (0);
}

int	execute_cmd(t_ast_node *ast, t_arena arena)
{
	t_command cmd;

	if (ast->type != AST_COMMAND)
	{
		ft_dprintf(2, "Error\n");
		return (1);
	}
	return (1);
}

