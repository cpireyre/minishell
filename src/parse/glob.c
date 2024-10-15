/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 20:02:19 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/14 14:32:15 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdbool.h>
#include "libft.h"
#include "expand.h"
#include "ast.h"

static char			*glob_pattern(t_arena arena, const char *str);
static bool			match(const char *pattern, const char *candidate);
static const char	**get_cwd_entries(t_arena arena);

/*TODO: Add return value and check for ENOMEM*/
void	glob(t_arena arena, t_ast_node *ast)
{
	size_t	i;
	char	*globbed;

	if (!ast)
		return ;
	i = 0;
	while (i < ast->n_children)
	{
		if (ast->children[i]->type == AST_WORD)
		{
			globbed = glob_pattern(arena, ast->children[i]->token.value);
			ast->children[i]->token.value = globbed;
			ast->children[i]->token.size = ft_strlen(globbed);
		}
		glob(arena, ast->children[i]);
		i++;
	}
}

static char	*glob_pattern(t_arena arena, const char *str)
{
	const char		**entries = get_cwd_entries(arena);
	t_string_vector	vec;

	if (!entries)
		return (NULL);
	ft_bzero(&vec, sizeof(vec));
	while (*entries)
	{
		if ((str[0] == '.' || **entries != '.') && match(str, *entries))
		{
			vec = realloc_maybe(arena, vec);
			vec.strings[vec.count++] = ft_arena_strndup(
					arena, *entries, ft_strlen(*entries));
			if (!vec.strings[vec.count - 1])
				return (NULL);
		}
		entries++;
	}
	if (!vec.count)
		return ((char *)str);
	return (ft_arena_strjoin_with_separator(
			arena, vec.strings, vec.count, ' '));
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

static const char	**get_cwd_entries(t_arena arena)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;
	const char		**result;

	count = 0;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	while (readdir(dir))
		count++;
	closedir(dir);
	result = arena_calloc(arena, count + 1, sizeof(char *));
	dir = opendir(".");
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (entry)
	{
		*result++ = ft_arena_strndup(
				arena, entry->d_name, ft_strlen(entry->d_name));
		entry = readdir(dir);
	}
	closedir(dir);
	return (result - count);
}
