/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 10:03:17 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/14 10:24:51 by copireyr         ###   ########.fr       */
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

extern void				expand(t_ast_node *ast, t_arena arena, t_list *env);
extern t_string_vector	realloc_maybe(t_arena arena, t_string_vector vec);

#endif /* EXPAND_H */
