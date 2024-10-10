/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 20:02:19 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/10 23:48:16 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include "libft.h"
#include "ast.h"

static bool	match(const char *pattern, const char *candidate);

/*If you don't find anything when globbing, keep the star*/

void	glob_str(const char *str)
{
	const DIR		*dir = opendir(getcwd(NULL, 0));
	struct dirent	*entry;

	if (dir)
	{
		while (1)
		{
			entry = readdir((DIR *)dir);
			if (!entry)
				break ;
			if (match(str, entry->d_name))
				ft_printf("%s\n", entry->d_name);
		}
		closedir((DIR *)dir);
	}
}

void	glob(t_ast_node *ast)
{
	size_t	i;

	if (!ast)
		return ;
	i = 0;
	while (i < ast->n_children)
	{
		if (ast->children[i]->type == AST_WORD)
		{
			const char *str = ast->children[i]->token.value;
			glob_str(str);
		}
		i++;
	}
	i = 0;
	while (i < ast->n_children)
	{
		glob(ast->children[i]);
		i++;
	}
}

static bool	match(const char *pattern, const char *candidate)
{
	if (!*pattern)
		return (!*candidate);
	if (*pattern == '*')
	{
		while (*pattern == '*')
			pattern++;
		if (!*pattern)
			return (true);
		while (*candidate)
			if (match(pattern, candidate++))
				return (true);
		return (false);
	}
	if (*pattern == *candidate)
		return (match(++pattern, ++candidate));
	return (false);
}
