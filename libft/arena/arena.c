/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:04:44 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/13 09:37:16 by copireyr         ###   ########.fr       */
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
	*arena = NULL;
	return (arena);
}

void	*arena_alloc(t_arena arena, size_t nbytes)
{
	t_arena_node	*new;
	void			*ptr;

	if (!arena || nbytes <= 0)
		return (NULL);
	new = malloc(sizeof(*new));
	if (!new)
		return (NULL);
	ptr = malloc(nbytes);
	if (!ptr)
	{
		free(new);
		return (NULL);
	}
	new->ptr = ptr;
	new->next = *arena;
	*arena = new;
	return (ptr);
}

void	*arena_calloc(t_arena arena, size_t count, size_t nbytes)
{
	void			*ptr;
	unsigned char	*pb;
	size_t			i;

	ptr = arena_alloc(arena, count * nbytes);
	if (ptr)
	{
		pb = (unsigned char *)ptr;
		i = 0;
		while (i < count * nbytes)
			pb[i++] = 0;
	}
	return (ptr);
}

void	arena_free(t_arena arena)
{
	t_arena_node	*current;
	t_arena_node	*next;

	if (!arena)
		return ;
	current = *arena;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	*arena = NULL;
}

void	arena_dispose(t_arena *ap)
{
	if (!ap || !*ap)
		return ;
	arena_free(*ap);
	free(*ap);
	*ap = NULL;
}
