/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2i_mul.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 09:56:15 by copireyr          #+#    #+#             */
/*   Updated: 2024/07/05 09:56:16 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_la.h"

t_v2i	v2i_mul(t_v2i a, t_v2i b)
{
	a.x *= b.x;
	a.y *= b.y;
	return (a);
}

t_v2i	v2i_div(t_v2i a, t_v2i b)
{
	a.x /= b.x;
	a.y /= b.y;
	return (a);
}

t_v2i	v2i_add(t_v2i a, t_v2i b)
{
	a.x += b.x;
	a.y += b.y;
	return (a);
}
