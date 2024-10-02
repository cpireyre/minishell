/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:58:29 by pleander          #+#    #+#             */
/*   Updated: 2024/10/01 16:49:13 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "tokenize.h"

enum e_ast_type
{
	AST_LOGICAL,
	AST_PIPELINE,
	AST_COMMAND,
	AST_WORD,
	AST_REDIR,
};

typedef struct	s_ast_node
{
	enum e_ast_type		type;
	struct s_ast_node	**children;
	t_token				token;
	size_t				n_children;
	int					exit_code;
}	t_ast_node;

t_ast_node	*create_ast(t_token *xs, t_ast_node *ast, size_t range[2], t_arena arena);
#endif
