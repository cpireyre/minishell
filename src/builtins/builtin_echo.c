/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:22:27 by pleander          #+#    #+#             */
/*   Updated: 2024/10/30 14:19:35 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	remove_outer_quotations(char *arg)
{
	char	*qs;
	char	*qe;
	int		i;

	qs = NULL;
	qe = NULL;
	i = 0;
	while (arg[i])
	{
		if (!qs && (arg[i] == '\"' || arg[i] == '\''))
			qs = &arg[i];
		else if (qs && arg[i] == *qs)
			qe = &arg[i];
		if (qs && qe)
			break ;
		i++;
	}
	if (!qe || !qs)
		return ;
	ft_memmove(qe, qe + 1, ft_strlen(qe + 1) + 1);
	ft_memmove(qs, qs + 1, ft_strlen(qs + 1) + 1);
}

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
			remove_outer_quotations(args[i]);
			ft_printf("%s ", args[i++]);
		}
		remove_outer_quotations(args[i]);
		ft_printf("%s", args[i]);
	}
	if (nl)
		ft_printf("%s", "\n");
	return (0);
}
