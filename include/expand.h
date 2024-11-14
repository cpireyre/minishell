/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 10:03:17 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/13 10:22:47 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "libft.h"
# include "ast.h"

typedef struct s_string_vector
{
	char	**strings;
	int		count;
	int		capacity;
}	t_string_vector;

typedef struct s_ast_vec
{
	t_ast_node	**data;
	size_t		size;
	size_t		capacity;
}	t_ast_vec;

bool					expand(t_ast_node *ast, t_arena arena,
							t_list *env, int exit_code);
extern t_string_vector	realloc_maybe(t_arena arena, t_string_vector vec);
t_ast_vec				*ast_push(t_arena arena,
							t_ast_vec *vec, t_ast_node *node);

#endif /* EXPAND_H */
