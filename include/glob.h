/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 20:08:57 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/14 10:00:11 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOB_H
# define GLOB_H

# include "libft.h"
# include "ast.h"

# define QUOTED_BIT (1 << 15)
# define CHAR_MASK  (0x7F)

typedef struct s_glob_result
{
	char	**matches;
	size_t	count;
}	t_glob_result;

typedef unsigned short t_quote;

extern void	glob(t_arena arena, t_ast_node *ast);
t_quote *quotes_lift(t_arena arena, const char *str);
char	*quotes_lower(t_arena arena, const t_quote *str);

#endif /* GLOB_H */
