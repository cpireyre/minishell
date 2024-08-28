/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_min4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:43:37 by copireyr          #+#    #+#             */
/*   Updated: 2024/08/02 12:43:39 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_min4(int a, int b, int c, int d)
{
	int	min;

	min = a;
	if (b < min)
		min = b;
	if (c < min)
		min = c;
	if (d < min)
		min = d;
	return (min);
}
