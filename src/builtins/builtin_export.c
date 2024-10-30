/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:15:09 by pleander          #+#    #+#             */
/*   Updated: 2024/10/30 15:07:48 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static int	is_numerical_id(char	*id)
{
	int	i;

	i = 0;
	while (id[i] && id[i] != '=')
	{
		if (!ft_isdigit(id[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	check_identifier(char *id)
{
	int	i;

	i = 0;
	while (id[i])
	{
		if ((i > 0 && id[i] == '='))
			break ;
		if (!ft_isalnum(id[i]))
			return (-1);
		i++;
	}
	if (is_numerical_id(id))
		return (-1);
	return (0);
}
int	export(char *export_str, t_list **env)
{
	char	*eq;
	char	*var;
	char	*val;
	int		ret;

	if (check_identifier(export_str) < 0)
	{
		ft_dprintf(2, "%s: export: '%s': not a valid identifier\n",
			NAME, export_str);
		return (1);
	}
	eq = ft_strchr(export_str, '=');
	if (!eq)
		return (0);
	var = ft_substr(export_str, 0, eq - export_str);
	if (!var)
		return (1);
	val = ft_substr(export_str, eq - export_str + 1, ft_strlen(eq + 1));
	ret = set_env(var, val, env);
	free(val);
	free(var);
	return (ret);
}
