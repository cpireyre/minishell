/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:50:16 by pleander          #+#    #+#             */
/*   Updated: 2024/09/25 13:20:03 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static void	del_only_element(t_list *del, t_list **env)
{
	ft_lstdelone(del, &free);
	*env = NULL;
}

static void	del_first_element(t_list *del, t_list **env)
{
	*env = del->next;
	ft_lstdelone(del, &free);
}

static void	del_last_element(t_list *del)
{
	del->prev->next = NULL;
	ft_lstdelone(del, &free);
}

static void	del_any_element(t_list *del)
{
	del->prev->next = del->next;
	del->next->prev = del->prev;
	ft_lstdelone(del, &free);
}

void	unset(char *var, t_list **env)
{
	t_list	*del;

	del = get_env_list_location(var, env);
	if (!del)
		return ;
	if (!del->next && !del->prev)
		del_only_element(del, env);
	else if (del->prev == NULL)
		del_first_element(del, env);
	else if (del->next == NULL)
		del_last_element(del);
	else
		del_any_element(del);
}
