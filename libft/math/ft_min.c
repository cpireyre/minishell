/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_min.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 13:32:46 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/01 12:07:21 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_min(int a, int b)
{
	if (a >= b)
		return (b);
	return (a);
}

size_t	ft_size_t_min(size_t a, size_t b)
{
	if (a >= b)
		return (b);
	return (a);
}
