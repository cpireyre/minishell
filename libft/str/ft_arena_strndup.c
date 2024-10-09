/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arena_strndup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 18:51:55 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/07 18:53:43 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_arena_strndup(t_arena arena, const char *src, size_t n)
{
	size_t	i;
	size_t	j;
	char	*dup;

	i = 0;
	while (src[i] && i < n)
		i++;
	dup = arena_alloc(arena, i + 1);
	if (!dup)
		return (NULL);
	j = 0;
	while (j < i)
	{
		dup[j] = src[j];
		j++;
	}
	dup[j] = '\0';
	return (dup);
}
