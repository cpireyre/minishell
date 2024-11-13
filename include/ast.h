/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:58:29 by pleander          #+#    #+#             */
/*   Updated: 2024/11/13 09:58:02 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "tokenize.h"

enum e_ast_type
{
	AST_LOGICAL = 0,
	AST_PAREN = 1,
	AST_PIPELINE = 2,
	AST_COMMAND = 3,
	AST_WORD = 4,
	AST_REDIR = 5,
	AST_NONE = 6,
};

typedef struct s_ast_node
{
	enum e_ast_type		type;
	struct s_ast_node	**children;
	t_token				token;
	size_t				n_children;
	int					exit_code;
}	t_ast_node;

t_ast_node	*create_ast(t_token *xs, t_ast_node *ast,
				size_t range[2], t_arena arena);
t_ast_node	*parse(t_arena arena, char *user_input_line,
				t_list *env, int exit_code);
void		print_ast(t_ast_node *root, size_t level);

#endif
