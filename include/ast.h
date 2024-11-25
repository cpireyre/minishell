/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:58:29 by pleander          #+#    #+#             */
/*   Updated: 2024/11/14 14:47:43 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "tokenize.h"

enum e_ast_type
{
	AST_LOGICAL = 0,
	AST_PIPELINE = 1,
	AST_PAREN = 2,
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
}	t_ast_node;

t_ast_node		*create_ast(t_token *xs, size_t range[2], t_arena arena);
t_ast_node		*parse(t_arena arena, char *user_input_line, t_list *env,
					int exit_code);
void			print_ast(t_ast_node *root, size_t level);
int				is_parenthesis(t_token x);
int				find_next_logical_token(t_token *xs, size_t range[2]);
int				find_next_token_of_type(t_token *xs, enum e_tok_type type,
					size_t range[2]);
size_t			count_tokens(t_token *xs, enum e_tok_type type,
					size_t range[2]);
void			show_token_range(t_token *xs, size_t range[2]);
enum e_ast_type	get_highest_ast_type(t_token *xs, size_t range[2]);
void			syntax_error(void);
t_ast_node		*create_command_node(t_token *xs, size_t range[2],
					t_arena arena);
t_ast_node		*parse(t_arena arena, char *user_input_line,
					t_list *env, int exit_code);
void			print_ast(t_ast_node *root, size_t level);
int				is_logical_token(t_token x);

#endif
