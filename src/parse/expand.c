/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 10:00:45 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/10 11:10:23 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ast.h"
#include <stdbool.h>

static char	*val(t_list *env, const char *key, size_t length_key);

/*
 * static variable is dangerous here, because the state will NOT
 * be wiped clean when we parse the next command. However we know
 * from the tokenizer that double quotes only occur in pairs, so
 * expand_anyway should always be false when we are done expanding.
*/

int	find_next_expandable(const char *str)
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

char	*expand_str(t_arena arena, t_list *env, const char *end)
{
	char		*substrings[256];
	int			count;
	const char	*start;

	count = 0;
	while (*end)
	{
		start = end;
		end += find_next_expandable(end);
		substrings[count++] = ft_arena_strndup(arena, start, end - start);
		if (!substrings[count - 1])
			return (NULL);
		if (*end == '$')
		{
			start = ++end;
			while (ft_isalnum(*end) || *end == '_')
				end++;
			substrings[count++] = val(env, start, end - start);
		}
	}
	return (ft_arena_strjoin(arena, substrings, count));
}

void	expand(t_ast_node *ast, t_arena arena, t_list *env)
{
	size_t	i;

	if (!ast)
		return ;
	i = 0;
	while (i < ast->n_children)
	{
		if (ast->children[i]->type == AST_WORD)
			ast->children[i]->token.value = expand_str(arena, env, ast->children[i]->token.value);
		i++;
	}
	i = 0;
	while (i < ast->n_children)
	{
		expand(ast->children[i], arena, env);
		i++;
	}
}

static char	*val(t_list *env, const char *key, size_t length_key)
{
	char	*curr;

	while (env && length_key)
	{
		size_t i = 0;
		curr = env->content;
		while (curr[i] != '=')
			i++;
		if (i == length_key && !ft_memcmp(key, curr, length_key))
			return (curr + i + 1);
		env = env->next;
	}
	return ("");
}
