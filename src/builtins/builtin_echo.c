/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:22:27 by pleander          #+#    #+#             */
/*   Updated: 2024/10/31 13:50:32 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	echo(char **args)
{
	int	nl;
	int	i;

	nl = 1;
	if (args && (args[1]))
	{
		i = 1;
		if (ft_streq(args[i], "-n"))
		{
			nl = 0;
			i++;
		}
		while (args[i + 1])
		{
			ft_printf("%s ", args[i++]);
		}
		ft_printf("%s", args[i]);
	}
	if (nl)
		ft_printf("%s", "\n");
	return (0);
}
