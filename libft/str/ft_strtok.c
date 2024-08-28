/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 09:55:42 by copireyr          #+#    #+#             */
/*   Updated: 2024/07/05 09:55:45 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

static int	is_sep(char c, const char *sep);

char	*ft_strtok(char *str, const char *sep)
{
	static char	*ptr;
	char		*ret;

	if (str)
		ptr = str;
	if (!ptr || !*ptr)
		return (NULL);
	while (is_sep(*ptr, sep))
		ptr++;
	if (!*ptr)
		return (NULL);
	ret = ptr;
	while (*ptr && !is_sep(*ptr, sep))
		ptr++;
	if (*ptr)
		*ptr++ = '\0';
	return (ret);
}

static int	is_sep(char c, const char *sep)
{
	while (*sep)
	{
		if (c == *sep++)
			return (1);
	}
	return (0);
}
