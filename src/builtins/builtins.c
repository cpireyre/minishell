/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 09:32:16 by pleander          #+#    #+#             */
/*   Updated: 2024/11/03 10:20:39 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	is_builtin(const char *name)
{
	if (!name)
		return (1);
	if (ft_streq(name, "env") || ft_streq(name, "export")
		|| ft_streq(name, "unset") || ft_streq(name, "pwd")
		|| ft_streq(name, "cd") || ft_streq(name, "echo")
		|| ft_streq(name, "exit"))
		return (1);
	return (0);
}
