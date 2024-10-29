/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:28:45 by pleander          #+#    #+#             */
/*   Updated: 2024/10/29 13:38:42 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

int	cd(char	*path)
{
	int ret;

	ft_printf("Attempting to cd to %s\n", path);
	ret = chdir(path);
	return (ret);
}
