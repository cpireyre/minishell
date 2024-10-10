/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:51:52 by pleander          #+#    #+#             */
/*   Updated: 2024/10/10 14:18:31 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arena.h"
#include "libft.h"
#include "ast.h"
#include "execute.h"
#include "minishell.h"

static int	execute_cmd(t_ast_node *ast, t_list *env, t_arena arena);

/**
 * @brief Executes the AST and returns the exit code
 *
 * @param ast AST node
 * @return exit code
 */
int	execute_ast(t_ast_node *ast, t_list	*env, t_arena arena)
{
	size_t	cur_child;

	if (ast->type == AST_COMMAND)
	{
		ft_printf("Execute AST command: ");
		ft_printf("%s\n", ast->token.value);
		execute_cmd(ast, env ,arena);
		return (0);
	}

	cur_child = 0;
	while (cur_child < ast->n_children)
	{
		if (ast->type == AST_PIPELINE)
		{
			ft_printf("Execute pipeline\n");
			execute_ast(ast->children[cur_child], env, arena);
		}
		else if (ast->type == AST_LOGICAL)
		{
			ft_printf("Execute logical statment\n");
			execute_ast(ast->children[cur_child], env, arena);
		}
		else
			ft_printf("We should not be here..\n");
		cur_child++;
	}
	return (0);
}

static int	execute_cmd(t_ast_node *ast, t_list *env, t_arena arena)
{
	t_command cmd;

	if (ast->type != AST_COMMAND)
	{
		ft_dprintf(2, "Error\n");
		return (1);
	}
	make_command(&cmd, ast, env, arena);
	return (1);
}

