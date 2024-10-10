/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 10:00:45 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/10 10:02:49 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ast.h"
#include <stdbool.h>

static char	*val(t_list *env, const char *key, size_t length_key);

char	*expand_str(t_arena arena, t_list *env, const char *str)
{
    bool	expand_anyway = false;
    char	*substrings[256];
    int		count_substrings = 0;
    t_arena	scratch = arena_new();
    const char	*ptr;

    if (!scratch)
	return (NULL);
    while (*str)
    {
	ptr = str;
	while (*str && *str != '$')
	{
	    if (*str == '"')
		expand_anyway = !expand_anyway;
	    if (*str == '\'' && !expand_anyway)
		str = ft_strchrnul(str + 1, '\'');
	    str++;
	}
	substrings[count_substrings++] = ft_arena_strndup(scratch, ptr, str - ptr);
	if (!substrings[count_substrings - 1])
	    return (NULL);
	if (*str++ != '$')
	    break ;
	ptr = str;
	while (ft_isalnum(*str) || *str == '_')
	    str++;
	char *expandable = ft_arena_strndup(scratch, ptr, str - ptr);
	char *match = val(env, expandable, str - ptr);
	if (match)
	    substrings[count_substrings++] = match;
    }
    char *result = ft_arena_strjoin(arena, substrings, count_substrings);
    arena_dispose(&scratch);
    return (result);
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

    while (env)
    {
	size_t i = 0;
	curr = env->content;
	while (curr[i] != '=')
	    i++;
	if (i == length_key && !ft_memcmp(key, curr, length_key))
	    return (curr + i + 1);
	env = env->next;
    }
    return (NULL);
}
