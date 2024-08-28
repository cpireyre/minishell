/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 12:43:03 by copireyr          #+#    #+#             */
/*   Updated: 2024/04/18 14:51:42 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*merged;
	size_t	merged_length;

	merged_length = ft_strlen(s2);
	if (s1)
		merged_length += ft_strlen(s1);
	merged = malloc(sizeof(char) * (merged_length + 1));
	if (!merged)
		return (NULL);
	if (s1)
		ft_strlcpy(merged, s1, merged_length + 1);
	ft_strlcat(merged, s2, merged_length + 1);
	return (merged);
}
