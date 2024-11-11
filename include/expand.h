/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 10:03:17 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/10 21:57:45 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "libft.h"
# include "ast.h"

typedef struct {
    t_quote *data;
    size_t len;
    size_t cap;
} t_quote_vec;

typedef struct s_ast_vec
{
	t_ast_node	**data;
	size_t		size;
	size_t		capacity;
}	t_ast_vec;

t_token *expand(t_arena arena, t_token *tokens, t_list *env, int exit_code);

#endif /* EXPAND_H */
