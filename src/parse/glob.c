/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 20:02:19 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/13 13:17:30 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdbool.h>
#include "libft.h"
#include "expand.h"
#include "glob.h"
#include "ast.h"

static bool			match(const t_quote *pattern, const char *candidate);
static const char	**get_cwd_entries(t_arena arena);

static const char	**glob_pattern(t_arena arena,
		const char **entries, const char *str)
{
	const t_quote	*glob_pattern = quotes_lift(arena, str);
	const char		**matches;
	size_t			count;
	const char		**curr = entries - 1;

	if (!entries || !glob_pattern)
		return (NULL);
	count = 0;
	while (*++curr)
	{
		if ((str[0] == '.' || **curr != '.') && match(glob_pattern, *curr))
			count++;
	}
	matches = arena_calloc(arena, count + !count + 1, sizeof(char *));
	if (!matches)
		return (NULL);
	if (!count)
		matches[0] = quotes_lower(arena, glob_pattern);
	while (*entries && count)
	{
		if ((*str == '.' || **entries != '.') && match(glob_pattern, *entries))
			*matches++ = *entries;
		entries++;
	}
	return (matches - count);
}

static bool	expand_glob_node(t_arena arena, const char **entries,
	t_ast_node *node, t_ast_vec *new_children)
{
	const char	**result;
	t_ast_node	*new_node;

	result = glob_pattern(arena, entries, node->token.value);
	while (result && *result)
	{
		new_node = arena_calloc(arena, 1, sizeof(*new_node));
		if (!new_node)
			return (false);
		new_node->type = AST_WORD;
		new_node->token.type = node->token.type;
		new_node->token.value = *result;
		new_node->token.size = ft_strlen(*result);
		new_node->token.is_globbed = true;
		if (!ast_push(arena, new_children, new_node))
			return (false);
		result++;
	}
	return (true);
}

void	glob(t_arena arena, t_ast_node *ast)
{
	const char	**entries = get_cwd_entries(arena);
	t_ast_vec	new_children;
	int			i;
	bool		has_glob;
	t_ast_node	*node;

	if (!ast || !entries)
		return ;
	i = -1;
	ft_bzero(&new_children, sizeof(new_children));
	while (++i < (int)ast->n_children)
	{
		node = ast->children[i];
		has_glob = ft_strchr(node->token.value, '*') != NULL;
		if (has_glob && node->type == AST_WORD
			&& !expand_glob_node(arena, entries, node, &new_children))
			return ;
		if (!has_glob && !ast_push(arena, &new_children, node))
			return ;
	}
	ast->children = new_children.data;
	ast->n_children = new_children.size;
	i = 0;
	while ((size_t)i < ast->n_children)
		glob(arena, ast->children[i++]);
}

static bool	match(const t_quote *pattern, const char *candidate)
{
	if (!*pattern)
		return (!*candidate);
	if ((*pattern & CHAR_MASK) == '*' && !(*pattern & QUOTED_BIT))
	{
		while ((*pattern & CHAR_MASK) == '*' && !(*pattern & QUOTED_BIT))
			pattern++;
		if (!*pattern)
			return (true);
		while (*candidate)
			if (match(pattern, candidate++))
				return (true);
		return (false);
	}
	if ((*pattern & CHAR_MASK) == *candidate)
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
