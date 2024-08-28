/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 10:12:40 by copireyr          #+#    #+#             */
/*   Updated: 2024/07/18 10:14:47 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	ft_bzero(void *s, size_t n);

void	*ft_calloc(size_t elems, size_t elem_size)
{
	void	*ret;

	ret = malloc(elems * elem_size);
	if (ret)
		ft_bzero(ret, elems * elem_size);
	return (ret);
}
