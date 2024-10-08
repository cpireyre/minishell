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

#include "libft.h"
#include "ast.h"

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
