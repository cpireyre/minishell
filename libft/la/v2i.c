/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2i.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 09:56:00 by copireyr          #+#    #+#             */
/*   Updated: 2024/07/05 12:36:15 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_la.h"

t_v2i	v2i(int x, int y)
{
	t_v2i	v;

	v.x = x;
	v.y = y;
	return (v);
}

t_v2i	v2i3d(t_v3d a)
{
	t_v2i	result;

	result.x = (int)a.x;
	result.y = (int)a.y;
	return (result);
}

int	v2i_sqrlen(t_v2i a)
{
	return (a.x * a.x + a.y * a.y);
}

t_v2i	v2i_sub(t_v2i a, t_v2i b)
{
	a.x -= b.x;
	a.y -= b.y;
	return (a);
}

int	v2i_eq(t_v2i a, t_v2i b)
{
	return (a.x == b.x && a.y == b.y);
}
