/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:17:24 by pleander          #+#    #+#             */
/*   Updated: 2024/09/03 15:23:13 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_puts(void *s)
{
	ft_printf("%s\n", (char *)s);
}

int	printenv(t_list **env)
{
	if (!env)
		return (-1);
	ft_lstiter(*env, &ft_puts);
	return (1);
}
