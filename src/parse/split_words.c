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

typedef struct s_ast_vec
{
	t_ast_node	**data;
	size_t		size;
	size_t		capacity;
}	t_ast_vec;

t_ast_vec	*vector_push(t_arena arena, t_ast_vec *vec, t_ast_node *node)
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

/* TODO: Check return val for ENOMEM */
void	split_words(t_arena arena, t_ast_node *ast)
{
	t_ast_vec	new_children;
	size_t		i;
	char		**split;

	if (!ast || !ast->children)
		return ;
	new_children = (t_ast_vec){0};
	i = 0;
	while (i < ast->n_children)
	{
		if (ast->children[i]->type == AST_WORD)
		{
			split = split_str(arena, ast->children[i]->token.value);
			while (*split)
				if (!vector_push(arena, &new_children, ast_from_str(arena, *split++)))
					return ;
		}
		else
			if (!vector_push(arena, &new_children, ast->children[i]))
				return ;
		i++;
	}
	ast->children = new_children.data;
	ast->n_children = new_children.size;
	i = 0;
	while (i < ast->n_children)
		split_words(arena, ast->children[i++]);
}
