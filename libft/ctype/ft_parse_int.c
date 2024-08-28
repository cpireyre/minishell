/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:43:53 by copireyr          #+#    #+#             */
/*   Updated: 2024/08/02 12:43:54 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

int	ft_parse_int(char *str, long *n)
{
	int	i;
	int	sign;
	int	digits;

	i = 0;
	sign = -1;
	digits = 0;
	while (str[i])
	{
		if (str[i] == '+' || str[i] == '-')
			sign = i;
		digits += ft_isdigit(str[i]);
		i++;
	}
	if (!digits || digits != i - (sign == 0))
		return (0);
	*n = ft_atol(str);
	return (INT_MIN <= *n && *n <= INT_MAX);
}
