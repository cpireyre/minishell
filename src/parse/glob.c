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

typedef unsigned short t_quote;
static bool			match(const t_quote *pattern, const char *candidate);
static const char	**get_cwd_entries(t_arena arena);

# define QUOTED_BIT (1 << 15)
# define CHAR_MASK  (0x7F)

t_quote *quotes_lift(t_arena arena, const char *str)
{
	t_quote	*result;
	int		j;
	char	current_quote;
	bool	should_toggle_quote;

	if (!str)
		return (NULL);
	result = arena_calloc(arena, ft_strlen(str) + 1, sizeof(*result));
	if (!result)
		return (NULL);
	j = 0;
	current_quote = 0;
	while (*str)
	{
		should_toggle_quote = (*str == '\'' || *str == '\"')
			&& (current_quote == 0 || *str == current_quote);
		if (should_toggle_quote)
			current_quote = !current_quote * *str;
		else
			result[j++] = *str | (!!current_quote * QUOTED_BIT);
		str++;
	}
	result[j] = '\0';
	return (result);
}

char	*quotes_lower(t_arena arena, const t_quote *str)
{
	char	*result;
	int		len;

	len = 0;
	while (str[len++])
		;
	result = arena_calloc(arena, len + 1, sizeof(*result));
	while (result && *str)
		*result++ = *str++ & CHAR_MASK;
	*result++ = '\0';
	return (result - len);
}
typedef struct s_ast_vec
{
	t_ast_node	**data;
	size_t		size;
	size_t		capacity;
}	t_ast_vec;

t_ast_vec	*ast_push(t_arena arena, t_ast_vec *vec, t_ast_node *node)
{
	const size_t	new_capacity = 2 * vec->capacity + 4;
	t_ast_node		**new_data;

	if (vec->size >= vec->capacity)
	{
		new_data = arena_alloc(arena, new_capacity * sizeof(t_ast_node *));
		if (!new_data)
			return (NULL);
		if (vec->data)
			ft_memcpy(new_data, vec->data, vec->size * sizeof(t_ast_node *));
		vec->data = new_data;
		vec->capacity = new_capacity;
	}
	vec->data[vec->size++] = node;
	return (vec);
}

char	*glob_pattern(t_arena arena, const char **entries, const char *str)
{
	const t_quote	*glob_pattern = quotes_lift(arena, str);
	t_string_vector	vec;

	if (!entries || !glob_pattern)
		return (NULL);
	ft_bzero(&vec, sizeof(vec));
	while (*entries)
	{
		if ((str[0] == '.' || **entries != '.') && match(glob_pattern, *entries))
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
		return (quotes_lower(arena, glob_pattern));
	return (ft_arena_strjoin_with_separator(
			arena, vec.strings, vec.count, ' '));
}


/*TODO: Add return value and check for ENOMEM*/
void	glob(t_arena arena, t_ast_node *ast)
{
	const char		**entries = get_cwd_entries(arena);
	size_t	i;
	char	*globbed;

	if (!ast || !entries)
		return ;
	i = 0;
	while (i < ast->n_children)
	{
		if (ast->children[i]->type == AST_WORD)
		{
			globbed = glob_pattern(arena, entries, ast->children[i]->token.value);
			ast->children[i]->token.value = globbed;
			ast->children[i]->token.size = ft_strlen(globbed);
		}
		glob(arena, ast->children[i]);
		i++;
	}
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
