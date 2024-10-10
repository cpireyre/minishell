/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arena_strjoin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:10:20 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/10 10:11:01 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_arena_strjoin(t_arena arena, char **substrings, int count)
{
	int		total_expanded_size;
	int		i;
	char	*result;
	char	*tmp;
	int		length_current_substring;

	total_expanded_size = 0;
	i = 0;
	while (i < count)
	{
		total_expanded_size += ft_strlen(substrings[i]);
		i++;
	}
	result = arena_calloc(arena, 1, total_expanded_size + 1);
	tmp = result;
	i = 0;
	while (result && i < count)
	{
		length_current_substring = ft_strlen(substrings[i]);
		result = ft_memcpy(result, substrings[i], length_current_substring);
		result += length_current_substring;
		i++;
	}
	result = tmp;
	return (tmp);
}
