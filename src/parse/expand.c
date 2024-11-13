/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 10:00:45 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/13 10:17:50 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "libft.h"

static char	*expand_str(t_arena arena, t_list *env,
				const char *end, const char *exit_code);
static int	find_next_expandable(const char *str);
static char	*val(t_list *env, const char *key, size_t length_key,
				const char *exit_code_str);

/*TODO: Check return values for ENOMEM*/
void	expand(t_ast_node *ast, t_arena arena, t_list *env, int exit_code)
{
	size_t		i;
	char		exit_code_str[12];

	if (!ast)
		return ;
	i = 0;
	ft_snprintf(exit_code_str, 12, "%d", exit_code);
	while (i < ast->n_children)
	{
		if (ast->children[i]->type == AST_WORD)
		{
			ast->children[i]->token.value = expand_str(
					arena, env, ast->children[i]->token.value, exit_code_str);
			ast->children[i]->token.size = ft_strlen(
					ast->children[i]->token.value);
		}
		expand(ast->children[i], arena, env, exit_code);
		i++;
	}
}

static char	*expand_str(t_arena arena, t_list *env,
		const char *end, const char *exit_code)
{
	t_string_vector	vec;
	const char		*start;

	ft_bzero(&vec, sizeof(vec));
	while (*end)
	{
		vec = realloc_maybe(arena, vec);
		if (!vec.strings)
			return (NULL);
		start = end;
		end += find_next_expandable(end);
		vec.strings[vec.count++] = ft_arena_strndup(arena, start, end - start);
		if (!vec.strings[vec.count - 1])
			return (NULL);
		if (*end == '$')
		{
			start = ++end;
			if (*start == '?')
				end++;
			while (*start != '?' && (ft_isalnum(*end) || *end == '_'))
				end++;
			vec.strings[vec.count++] = val(env, start, end - start, exit_code);
		}
	}
	return (ft_arena_strjoin(arena, vec.strings, vec.count));
}

t_string_vector	realloc_maybe(t_arena arena, t_string_vector vec)
{
	char	**tmp;

	if (vec.count < vec.capacity)
		return (vec);
	if (!vec.capacity)
		vec.capacity = 8;
	vec.capacity = 2 * vec.capacity;
	tmp = arena_calloc(arena, vec.capacity, sizeof(char *));
	if (tmp)
		ft_memcpy(tmp, vec.strings, sizeof(char *) * vec.count);
	vec.strings = tmp;
	return (vec);
}

/*
 * static variable is dangerous here, because the state will NOT
 * be wiped clean when we parse the next command. However we know
 * from the tokenizer that double quotes only occur in pairs, so
 * expand_anyway should always be false when we are done expanding.
 */

static int	find_next_expandable(const char *str)
{
	static bool	expand_anyway = false;
	int			i;

	i = 0;
	while (str[i] && str[i] != '$')
	{
		if (str[i] == '"')
			expand_anyway = !expand_anyway;
		if (str[i] == '\'' && !expand_anyway)
			while (str[++i] != '\'')
				;
		i++;
	}
	return (i);
}

static char	*val(t_list *env, const char *key, size_t length_key,
			const char *exit_code_str)
{
	char	*curr;
	size_t	i;

	if (*key == '?' && length_key == 1)
		return ((char *)exit_code_str);
	if (!length_key)
		return ("$");
	while (env && length_key)
	{
		i = 0;
		curr = env->content;
		while (curr[i] != '=')
			i++;
		if (i == length_key && !ft_memcmp(key, curr, length_key))
			return (curr + i + 1);
		env = env->next;
	}
	return ("");
}
