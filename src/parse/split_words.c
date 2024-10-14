/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:54:15 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/14 14:29:31 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

int	count_words(const char *str)
{
	int			count;

	count = 0;
	while (*str)
	{
		while (*str && !ft_isspace(*str))
		{
			if (*str == '"')
				str = ft_strchrnul(str + 1, '"');
			if (*str == '\'')
				str = ft_strchrnul(str + 1, '\'');
			str++;
		}
		count++;
		while (ft_isspace(*str))
			str++;
	}
	return (count);
}

char	**split_str(t_arena arena, const char *str)
{
	const char	*start_of_word;
	const int	count = count_words(str);
	char		**result;

	result = arena_calloc(arena, count + 1, sizeof(*result));
	if (!result)
		return (NULL);
	while (*str)
	{
		start_of_word = str;
		while (*str && !ft_isspace(*str))
		{
			if (*str == '"')
				str = ft_strchrnul(str + 1, '"');
			if (*str == '\'')
				str = ft_strchrnul(str + 1, '\'');
			str++;
		}
		*result++ = ft_arena_strndup(arena, start_of_word, str - start_of_word);
		while (ft_isspace(*str))
			str++;
	}
	return (result - count);
}

void	split_words(t_arena arena, t_ast_node *ast)
{
	size_t	i;
	char	**split;

	if (!ast)
		return ;
	i = 0;
	while (i < ast->n_children)
	{
		if (ast->children[i]->type == AST_WORD)
		{
			split = split_str(arena, ast->children[i]->token.value);
			while (*split)
				ft_printf("%s\n", *split++);
		}
		split_words(arena, ast->children[i]);
		i++;
	}
}
