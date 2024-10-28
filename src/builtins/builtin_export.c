/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:15:09 by pleander          #+#    #+#             */
/*   Updated: 2024/10/28 10:47:23 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int	export(char *export_str, t_list **env)
{
	char	*eq;
	char	*var;
	char	*val;
	int		ret;

	eq = ft_strchr(export_str, '=');
	if (!eq)
		return (1);
	var = ft_substr(export_str, 0, eq - export_str);
	if (!var)
		return (1);
	if (ft_strchr(var, ' '))
	{
		ft_dprintf(2, "%s: export: '%s': not a valid identifier\n",
			NAME, export_str);
		free(var);
		return (1);
	}
	val = ft_substr(export_str, eq - export_str + 1, ft_strlen(eq + 1));
	ret = set_env(var, val, env);
	free(val);
	free(var);
	return (ret);
}
