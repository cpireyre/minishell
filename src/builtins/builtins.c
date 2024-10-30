/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 09:32:16 by pleander          #+#    #+#             */
/*   Updated: 2024/10/30 12:50:13 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	is_builtin(const char *name)
{
	if (ft_streq(name, "env") || ft_streq(name, "export") 
		|| ft_streq(name, "unset") || ft_streq(name, "pwd")
		|| ft_streq(name, "cd") || ft_streq(name, "echo")
		|| ft_streq(name, "exit"))
		return (1);
	return (0);
}
