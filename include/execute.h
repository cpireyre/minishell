/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:03:23 by pleander          #+#    #+#             */
/*   Updated: 2024/10/10 14:05:19 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

#include "ast.h"

typedef struct s_command
{
	char	*path;
	const char	**args;
	int		infile;
	int		outfile;

}	t_command;

typedef struct	s_command_context
{
	t_ast_node	*ast;
	t_list		*env;
	int			**pipes;
	int			cur_child;
}	t_command_context;

int		execute_ast(t_ast_node *ast, t_list	*env, t_arena arena);
int		make_command(t_command *cmd, t_ast_node *ast, t_list *env, t_arena arena);
int		**make_pipes(int n_pipes, t_arena arena);
void	close_pipes(int **pipes, int n_pipes);

#endif
