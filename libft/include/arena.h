/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:04:53 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/08 11:34:02 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARENA_H
# define ARENA_H

#include <stddef.h>

typedef struct s_arena_node
{
	void				*ptr;
	struct s_arena_node	*next;
}	t_arena_node;

typedef t_arena_node**	t_arena;

t_arena	arena_new(void);
void	arena_dispose(t_arena *ap);
void	*arena_alloc(t_arena arena, size_t nbytes);
void	*arena_calloc(t_arena arena, size_t count, size_t nbytes);

#endif /* ARENA_H */
