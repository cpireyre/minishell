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

/* AST_WORD nodes don't branch so no need to fill that in */
t_ast_node	*ast_from_str(t_arena arena, const char *str)
{
	t_ast_node	*result;

	result = arena_calloc(arena, 1, sizeof(*result));
	if (!result)
		return (NULL);
	result->type = AST_WORD;
	result->token.value = str;
	result->token.size = ft_strlen(str);
	return (result);
}

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
	if (!ast)
		return ;
	size_t	count = 0;
	t_ast_node	**new_children = arena_calloc(arena, 256, sizeof(*new_children));
	for (size_t i = 0; i < ast->n_children; i++)
	{
		if (ast->children[i]->type == AST_WORD)
		{
			char **split = split_str(arena, ast->children[i]->token.value);
			while (*split)
				new_children[count++] = ast_from_str(arena, *split++);
		}
		else
			new_children[count++] = ast->children[i];
	}
	ast->children = new_children;
	ast->n_children = count;
	for (size_t i = 0; i < ast->n_children; i++)
		split_words(arena, ast->children[i]);
}
