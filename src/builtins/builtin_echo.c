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

static bool	is_n_flag(const char *arg);

int	echo(char **args)
{
	int	nl;
	int	i;

	nl = 1;
	if (args && (args[1]))
	{
		i = 1;
		while (args[i] && is_n_flag(args[i]))
		{
			nl = 0;
			i++;
		}
		while (args[i])
		{
			ft_printf("%s", args[i++]);
			if (args[i])
				ft_printf(" ");
		}
	}
	if (nl)
		ft_printf("%s", "\n");
	return (0);
}

static bool	is_n_flag(const char *arg)
{
	if (!arg || arg[0] != '-' || !arg[1])
		return (false);
	while (*++arg == 'n')
		;
	return (!*arg);
}
