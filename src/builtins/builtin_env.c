/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:17:24 by pleander          #+#    #+#             */
/*   Updated: 2024/11/08 09:15:20 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int	printenv(t_list **env)
{
	t_list	*tmp;
	char	*val;
	size_t	len_val;

	tmp = *env;
	if (!env)
		return (1);
	while (tmp)
	{
		val = (char *)tmp->content;
		len_val = ft_strlen(val);
		if (val[len_val - 1] != '=')
			ft_printf("%s\n", val);
		tmp = tmp->next;
	}
	ft_printf("%s", ANSI_RESET);
	return (0);
}
