/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 20:02:19 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/31 13:46:22 by copireyr         ###   ########.fr       */
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

static const char	**glob_pattern(t_arena arena, const char **entries, const char *str)
{
    const t_quote	*glob_pattern = quotes_lift(arena, str);
    const char	**result = {0};
    const char		**matches;
    size_t			count = 0;

    if (!entries || !glob_pattern)
        return (result);
    const char **curr = entries;
    while (*curr)
    {
        if ((str[0] == '.' || **curr != '.') && match(glob_pattern, *curr))
            count++;
        curr++;
    }
    if (!count)
    {
        matches = arena_alloc(arena, 2 * sizeof(char *));
        if (!matches)
            return result;
        matches[0] = (char *)str;
        matches[1] = NULL;
        return (matches);
    }
    matches = arena_calloc(arena, count + 1, sizeof(char *));
    if (!matches)
        return result;
    while (*entries)
    {
        if ((str[0] == '.' || **entries != '.') && match(glob_pattern, *entries))
            *matches++ = *entries;
        entries++;
    }
	return (matches - count);
}

static t_ast_node *create_word_node(t_arena arena, const char *value,
                                  enum e_tok_type type, bool is_globbed)
{
    t_ast_node *node;

    node = arena_calloc(arena, 1, sizeof(*node));
    if (!node)
        return (NULL);
    node->type = AST_WORD;
    node->token.type = type;
    node->token.value = value;
    node->token.size = ft_strlen(value);
    node->token.is_globbed = is_globbed;
    return (node);
}

void    glob(t_arena arena, t_ast_node *ast)
{
    const char      **entries = get_cwd_entries(arena);
    t_ast_vec       new_children = {0};
    t_ast_node      *new_child;
    const char		**result;
    size_t          i;

    if (!ast || !entries)
        return ;
    i = 0;
    while (i < ast->n_children)
    {
        if (ast->children[i]->type == AST_WORD
            && ft_strchr(ast->children[i]->token.value, '*'))
        {
            result = glob_pattern(arena, entries, ast->children[i]->token.value);
			while (*result)
            {
				new_child = create_word_node(arena, *result++,
							ast->children[i]->token.type, true);
				if (!new_child || !ast_push(arena, &new_children, new_child))
					return ;
            }
        }
        else if (!ast_push(arena, &new_children, ast->children[i]))
            return ;
        i++;
    }
    ast->children = new_children.data;
    ast->n_children = new_children.size;
	i = 0;
    while (i < ast->n_children)
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
