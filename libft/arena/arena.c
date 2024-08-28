/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:04:44 by copireyr          #+#    #+#             */
/*   Updated: 2024/06/26 15:49:59 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "arena.h"

t_arena	arena_new(void)
{
	t_arena	arena;

	arena = malloc(sizeof(*arena));
	if (!arena)
		return (NULL);
	arena->prev = NULL;
	arena->limit = NULL;
	arena->avail = NULL;
	return (arena);
}

void	arena_dispose(t_arena *ap)
{
	arena_free(*ap);
	free(*ap);
	*ap = NULL;
}

void	*arena_alloc(t_arena arena, size_t nbytes)
{
	t_arena	ptr;
	char	*limit;
	size_t	m;

	if ((int)nbytes < 0)
		return (NULL);
	nbytes = ((nbytes + sizeof(union u_align) - 1)
			/ (sizeof(union u_align))) * (sizeof(union u_align));
	if (nbytes > (size_t)(arena->limit - arena->avail))
	{
		m = sizeof(union u_header) + nbytes + 10 * 1024;
		ptr = malloc(m);
		if (ptr == NULL)
			return (NULL);
		limit = (char *)ptr + m;
		*ptr = *arena;
		arena->avail = (char *)((union u_header *)ptr + 1);
		arena->limit = limit;
		arena->prev = ptr;
	}
	arena->avail += nbytes;
	return (arena->avail - nbytes);
}

void	ft_bzero(void *s, size_t n);

void	*arena_calloc(t_arena arena, size_t count, size_t nbytes)
{
	void	*ptr;

	if ((int)count < 0)
		return (NULL);
	ptr = arena_alloc(arena, count * nbytes);
	if (ptr)
		ft_bzero(ptr, count * nbytes);
	return (ptr);
}

void	arena_free(t_arena arena)
{
	struct s_arena	tmp;

	if (arena)
	{
		while (arena->prev)
		{
			tmp = *arena->prev;
			free(arena->prev);
			*arena = tmp;
		}
	}
}
