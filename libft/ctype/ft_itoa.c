/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:05:52 by pleander          #+#    #+#             */
/*   Updated: 2024/11/01 09:40:29 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_itoa_convert(int n, char *str, int sign)
{
	char	c;

	if (sign < 0)
		if (n < (9 / sign))
			ft_itoa_convert(n / 10, str, sign);
	if (sign > 0)
		if (n > (9 / sign))
			ft_itoa_convert(n / 10, str, sign);
	c = (n % 10) * sign + '0';
	str[ft_strlen(str)] = c;
}

static size_t	count(long n, int sign)
{
	size_t	c;

	c = 1;
	while (n * sign > 9)
	{
		c++;
		n = n / 10;
	}
	if (sign < 0)
		c++;
	return (c);
}

char	*ft_itoa(int n)
{
	char	*str;
	size_t	size;
	int		sign;

	sign = 1;
	if (n < 0)
		sign = -1;
	size = count(n, sign) + 1;
	str = ft_calloc(size, sizeof(char));
	if (!str)
		return (NULL);
	if (sign < 0)
		str[0] = '-';
	ft_itoa_convert((long)n, str, sign);
	return (str);
}

char	*ft_arena_itoa(t_arena arena, int n)
{
	char	*str;
	size_t	size;
	int		sign;

	sign = 1;
	if (n < 0)
		sign = -1;
	size = count(n, sign) + 1;
	str = arena_calloc(arena, size, sizeof(char));
	if (!str)
		return (NULL);
	if (sign < 0)
		str[0] = '-';
	ft_itoa_convert((long)n, str, sign);
	return (str);
}
