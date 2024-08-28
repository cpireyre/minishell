/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v3i.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 09:56:31 by copireyr          #+#    #+#             */
/*   Updated: 2024/07/05 12:23:04 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_la.h"

t_v3i	v3i(int x, int y, int z)
{
	t_v3i	result;

	result.x = x;
	result.y = y;
	result.z = z;
	return (result);
}

t_v3i	v3i_add(t_v3i a, t_v3i b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return (a);
}

t_v3i	v3i_mul(t_v3i a, t_v3i b)
{
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	return (a);
}

t_v3i	v3ii(int a)
{
	t_v3i	result;

	result.x = a;
	result.y = a;
	result.z = a;
	return (result);
}
