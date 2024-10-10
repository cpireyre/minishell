/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 10:00:45 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/10 09:35:54 by copireyr         ###   ########.fr       */
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
    const char	*ptr = str;
    if (!scratch)
	return (NULL);
    while (*str)
    {
	while (*str && *str != '$')
	{
	    if (*str == '"')
		expand_anyway = !expand_anyway;
	    if (*str == '\'' && !expand_anyway)
		str = ft_strchrnul(str + 1, '\'');
	    str++;
	}
	int size_current_substring = str - ptr;
	if (size_current_substring)
	{
	    char *substring = ft_arena_strndup(scratch, ptr, size_current_substring);
	    if (!substring)
		return (NULL);
	    substrings[count_substrings++] = substring;
	}
	if (*str++ != '$')
	    break ;
	size_t length_expandable = 0;
	while (ft_isalnum(str[length_expandable]) || str[length_expandable] == '_')
	    length_expandable++;
	char *expandable = ft_arena_strndup(scratch, str, length_expandable);
	str += length_expandable;
	char *match = val(env, expandable, length_expandable);
	if (match)
	    substrings[count_substrings++] = match;
	ptr = str;
    }
    int total_expanded_size = 0;
    for (int i = 0; i < count_substrings; i++)
	total_expanded_size += ft_strlen(substrings[i]);
    char *result = arena_calloc(arena, 1, total_expanded_size + 1);
    char *tmp = result;
    for (int i = 0; i < count_substrings; i++)
    {
	int length_current_substring = ft_strlen(substrings[i]);
	result = ft_memcpy(result, substrings[i], length_current_substring);
	result += length_current_substring;
    }
    result = tmp;
    arena_dispose(&scratch);
    return (result);
}

void	expand(t_ast_node *ast, t_arena arena, t_list *env)
{
    if (!ast)
	return ;
    for (size_t i = 0; i < ast->n_children; i++)
    {
	if (ast->children[i]->type == AST_WORD)
	    ast->children[i]->token.value = expand_str(arena, env, ast->children[i]->token.value);
    }
    for (size_t i = 0; i < ast->n_children; i++)
	expand(ast->children[i], arena, env);
}

static char	*val(t_list *env, const char *key, size_t length_key)
{
    while (env)
    {
	size_t i = 0;
	char *curr = env->content;
	while (curr[i] != '=')
	    i++;
	if (i == length_key && !ft_memcmp(key, curr, length_key))
	    return (curr + i + 1);
	env = env->next;
    }
    return (NULL);
}
