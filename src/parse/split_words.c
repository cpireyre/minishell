/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:54:15 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/16 09:55:11 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "expand.h"

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

char	*remove_quotes_from_str(t_arena arena, const char *str);

t_ast_vec	*expand_children(
		t_arena arena, const char *str, t_ast_vec *new_children)
{
	char		**split;
	t_ast_node	*new_child;

	split = split_str(arena, str);
	while (*split)
	{
		new_child = arena_calloc(arena, 1, sizeof(*new_child));
		if (!new_child)
			return (NULL);
		new_child->type = AST_WORD;
		new_child->token.value = *split;
		new_child->token.size = ft_strlen(new_child->token.value);
		if (!ast_push(arena, new_children, new_child))
			return (NULL);
		split++;
	}
	return (new_children);
}

/* TODO: Check return val for ENOMEM */
void	split_words(t_arena arena, t_ast_node *ast)
{
	t_ast_vec	new_children;
	size_t		i;
	const char	*str;

	if (!ast || !ast->children)
		return ;
	ft_bzero(&new_children, sizeof(new_children));
	i = 0;
	while (i < ast->n_children)
	{
		if (ast->children[i]->type == AST_WORD
		&& ast->children[i]->token.is_globbed == false)
		{
			str = ast->children[i]->token.value;
			if (!expand_children(arena, str, &new_children))
				return ;
		}
		else if (!ast_push(arena, &new_children, ast->children[i]))
			return ;
		i++;
	}
	ast->children = new_children.data;
	ast->n_children = new_children.size;
	i = 0;
	while (i < ast->n_children)
		split_words(arena, ast->children[i++]);
}
