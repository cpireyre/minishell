/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset_32.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 13:31:36 by copireyr          #+#    #+#             */
/*   Updated: 2024/06/26 13:31:41 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>

void	*ft_memset_32(void *b, uint32_t c, size_t len)
{
	uint32_t	*pb;

	pb = (uint32_t *)b;
	while (len--)
		*pb++ = c;
	return (b);
}
