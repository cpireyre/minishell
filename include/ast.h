/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:58:29 by pleander          #+#    #+#             */
/*   Updated: 2024/10/28 11:53:48 by pleander         ###   ########.fr       */
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
t_ast_node	*parse(t_arena arena, char *user_input_line, t_list *env, int exit_code);
void		print_ast(t_ast_node *root, size_t level);

/* t_ast		*new_node_from_token(t_arena arena, t_token token); */

#endif
