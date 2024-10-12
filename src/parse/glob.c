/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 20:02:19 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/12 16:00:09 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include "libft.h"
#include "expand.h"
#include "ast.h"

static bool	match(const char *pattern, const char *candidate);

/*If you don't find anything when globbing, keep the star*/

void	glob_str(t_arena arena, const char *str)
{
	t_string_vector	vec;
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(".");
	if (!dir)
		return ;
	vec = (t_string_vector){0};
	entry = readdir(dir);
	while (entry)
	{
		if ((str[0] == '.' || entry->d_name[0] != '.') && match(str, entry->d_name))
		{
			vec = realloc_maybe(arena, vec);
			vec.strings[vec.count++] = ft_arena_strndup(arena, entry->d_name, entry->d_namlen);
			if (!vec.strings[vec.count - 1])
				return ;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	ft_printf("%s", ft_arena_strjoin_with_separator(arena, vec.strings, vec.count, ' '));
}

void	glob(t_ast_node *ast)
{
	size_t	i;

	t_arena scratch = arena_new();
	if (!ast)
		return ;
	i = 0;
	while (i < ast->n_children)
	{
		if (ast->children[i]->type == AST_WORD)
			glob_str(scratch, ast->children[i]->token.value);
		glob(ast->children[i]);
		i++;
	}
	arena_dispose(&scratch);
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
