/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:03:23 by pleander          #+#    #+#             */
/*   Updated: 2024/10/29 15:08:06 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

#include "ast.h"

typedef struct s_command
{
	char	*path;
	char	**args;
	char	*infile;
	char	*outfile;
	int		infile_fd;
	int		outfile_fd;

}	t_command;

typedef struct	s_command_context
{
	t_ast_node	*ast;
	t_list		*env;
	int			**pipes;
	size_t		cur_child;
	size_t		n_children;
}	t_command_context;

int		execute_ast(t_ast_node *ast, t_list	*env, t_arena arena);
int		make_command(t_command *cmd, t_ast_node *ast, t_list *env, t_arena arena);
int		**make_pipes(int n_pipes, t_arena arena);
void	close_pipes(int **pipes, int n_pipes);
size_t	calculate_n_pipes(t_ast_node *ast);
int		run_builtin(char *builtin, char **args, t_list **env);

#endif
