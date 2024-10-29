/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 12:31:14 by pleander          #+#    #+#             */
/*   Updated: 2024/10/29 12:46:13 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unistd.h"
#include "libft.h"

int	pwd(void)
{
	size_t	buf_size;
	char	*buf;

	buf_size = 255;
	buf = malloc(buf_size);
	if (!buf)
		return (1);
	getcwd(buf, buf_size);
	while (!buf)
	{
		free(buf);
		buf_size += 255;
		buf = malloc(buf_size);
		if (!buf)
			return (1);
		getcwd(buf, buf_size);
	}
	ft_printf("%s\n", buf);
	free(buf);
	return (0);
}
