/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:15:09 by pleander          #+#    #+#             */
/*   Updated: 2024/11/08 09:32:07 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static int	is_numerical(char *id)
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

static int	check_character_invalid(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (0);
	return (1);
}

static int	check_identifier(char *id)
{
	int	i;

	i = 0;
	if (ft_isdigit(id[i]))
		return (-1);
	while (id[i])
	{
		if ((i > 0 && id[i] == '='))
			break ;
		if (check_character_invalid(id[i]))
			return (-1);
		i++;
	}
	if (is_numerical(id))
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
		return (set_env(export_str, "", env));
	var = ft_substr(export_str, 0, eq - export_str);
	if (!var)
		return (1);
	val = ft_substr(export_str, eq - export_str + 1, ft_strlen(eq + 1));
	ret = set_env(var, val, env);
	free(val);
	free(var);
	return (ret);
}

int	list_exports(t_list *tmp)
{
	char	*val;
	char	*key;

	while (tmp && tmp->content)
	{
		key = (char *)tmp->content;
		printf("declare -x ");
		val = ft_strchr(key, '=');
		if (val && *(val + 1))
			printf("%.*s=\"%s\"", (int)(val - key), key, val + 1);
		else
			printf("%.*s", (int)(val - key), key);
		printf("\n");
		tmp = tmp->next;
	}
	return (0);
}
