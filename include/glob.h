/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 20:08:57 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/13 10:02:41 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOB_H
# define GLOB_H

# include "libft.h"
# include "ast.h"

/* Really 1 << 15, but bit shifting in macro not norm compliant...? */
# define QUOTED_BIT 32768
# define CHAR_MASK  0x7F

typedef unsigned short	t_quote;

extern void	glob(t_arena arena, t_ast_node *ast);
t_quote		*quotes_lift(t_arena arena, const char *str);
char		*quotes_lower(t_arena arena, const t_quote *str);

#endif /* GLOB_H */
