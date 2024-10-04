/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 10:00:45 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/04 13:15:06 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ast.h"

char	*get_ast_type(enum e_ast_type t);

int	ft_isalnum(char c)
{
	return (('a' <= c && c <= 'z')
		|| ('A' <= c && c <= 'Z')
		|| ('0' <= c && c <= '9'));
}

char	*find_expandable(t_arena arena, t_token token)
{
	const char	*str;

	str = token.value;
	while (*str && *str != '$')
	{
		if (*str == '\'')
			str = ft_strchrnul(str + 1, '\'');
		str++;
	}
	if (*str++ != '$')
		return (NULL);
	size_t i = 0;
	while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	char *result = arena_calloc(arena, 1, i + 1);
	if (result)
		ft_memcpy(result, str, i);
	return (result);
}

char	*val(t_list *env, const char *key, size_t length_key)
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

void	expand(t_ast_node *ast, t_arena arena, t_list *env)
{
	const char		*expandable = find_expandable(arena, ast->token);
	if (expandable)
	{
		const size_t	length_expandable = ft_strlen(expandable);
		ft_printf("Found expandable: [%s]\n", expandable);
		const char *match = val(env, expandable, length_expandable);
		if (match)
			ft_printf("%s -> %s\n", expandable, match);
	}
}
