/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:17:24 by pleander          #+#    #+#             */
/*   Updated: 2024/09/25 10:53:46 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int	printenv(t_list **env)
{
	t_list	*tmp;

	tmp = *env;
	if (!env)
		return (-1);
	while (tmp)
	{
		if ((tmp)->next)
			ft_printf("%s\n", (char *)(tmp)->content);
		else
			ft_printf("%s%s\n", (char *)(tmp)->content, ANSI_RESET);
		tmp = (tmp)->next;
	}
	return (1);
}
