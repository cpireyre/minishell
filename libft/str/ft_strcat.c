/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 13:31:25 by copireyr          #+#    #+#             */
/*   Updated: 2024/05/27 13:31:26 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strcat(char *dst, const char *src)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (dst[i])
		i++;
	j = 0;
	while (src[j])
		dst[i++] = src[j++];
	dst[i] = '\0';
	return (dst);
}
