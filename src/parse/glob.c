/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 20:02:19 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/11 10:25:17 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include "libft.h"
#include "ast.h"

static bool	match(const char *pattern, const char *candidate);

/*If you don't find anything when globbing, keep the star*/

void	glob_str(const char *str)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(".");
	if (!dir)
		return ;
	entry = readdir(dir);
	while (entry)
	{
		if ((str[0] == '.' || entry->d_name[0] != '.') && match(str, entry->d_name))
		{
			ft_printf("%s ", entry->d_name);
		}
		entry = readdir(dir);
	}
	closedir(dir);
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
			glob_str(ast->children[i]->token.value);
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
