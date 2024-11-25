/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:51:54 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/14 13:34:35 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenize.h"
#include <errno.h>

static bool				get_operators(
							t_arena arena, t_token input, t_token_vec *vec);
static t_token			token_next(t_arena arena, const char *str);
static bool				token_vec_grow(t_arena arena, t_token_vec *vec);

t_token	*tokenize(t_arena arena, const char *str)
{
	t_token_vec		vec;
	t_token			next;

	ft_bzero(&vec, sizeof(vec));
	while (1)
	{
		while (ft_isspace(*str))
			str++;
		next = token_next(arena, str);
		if (next.type == TOK_META)
			get_operators(arena, next, &vec);
		else
		{
			if (!token_vec_grow(arena, &vec)
				|| (next.type == TOK_ERROR && errno == ENOMEM))
				return (NULL);
			vec.data[vec.len++] = next;
		}
		if (next.type == TOK_END)
			break ;
		str += next.size;
	}
	if (DEBUG)
		tokenize_show_tokens(vec.data);
	return (vec.data);
}

static bool	token_vec_grow(t_arena arena, t_token_vec *vec)
{
	t_token		*tmp;

	if (vec->len < vec->capacity)
		return (true);
	vec->capacity = 2 * vec->capacity + 1;
	tmp = arena_calloc(arena, vec->capacity, sizeof(t_token));
	if (!tmp)
		return (false);
	if (vec->data)
		ft_memcpy(tmp, vec->data, sizeof(t_token) * vec->len);
	vec->data = tmp;
	return (true);
}

static t_token	token_next(t_arena arena, const char *str)
{
	t_token	result;

	result = (t_token){token_get_type(*str), str, 0, false};
	while (result.type != TOK_END && token_get_type(*str) == result.type)
	{
		if (result.type == TOK_WORD && *str == '"')
		{
			str = ft_strchrnul(str + 1, '"');
			if (*str != '"')
				result.type = TOK_ERROR;
		}
		if (result.type == TOK_WORD && *str == '\'')
		{
			str = ft_strchrnul(str + 1, '\'');
			if (*str != '\'')
				result.type = TOK_ERROR;
		}
		if (result.type != TOK_ERROR)
			str++;
	}
	result.size = str - result.value;
	result.value = ft_arena_strndup(arena, result.value, result.size);
	if (!result.value)
		result.type = TOK_ERROR;
	return (result);
}

static t_token	match_operator(t_arena arena, const char *str, size_t remaining)
{
	static const struct s_operator	operators[] = {
	{">>",	2, TOK_APPEND}, {"<<",	2, TOK_HEREDOC},
	{"||",	2, TOK_LOGICAL_OR}, {"&&",	2, TOK_LOGICAL_AND},
	{">",	1, TOK_REDIRECT_OUT}, {"<",	1, TOK_REDIRECT_IN},
	{"|",	1, TOK_PIPE},
	{"(",	1, TOK_OPEN_PAREN}, {")",	1, TOK_CLOSE_PAREN},
	{"&",	1, TOK_ERROR}, {NULL,	0, TOK_ERROR}
	};
	int								i;

	i = 0;
	while (operators[i].str)
	{
		if (remaining >= operators[i].len
			&& !ft_strncmp(str, operators[i].str, operators[i].len))
			return ((t_token){
				.value = ft_arena_strndup(arena, str, operators[i].len),
				.size = operators[i].len, .type = operators[i].type});
		i++;
	}
	return ((t_token){.type = TOK_ERROR});
}

static bool	get_operators(t_arena arena, t_token input, t_token_vec *vec)
{
	const char	*str;
	size_t		remaining;
	t_token		tok;

	str = input.value;
	remaining = input.size;
	while (remaining > 0)
	{
		if (!token_vec_grow(arena, vec))
			return (false);
		tok = match_operator(arena, str, remaining);
		vec->data[vec->len++] = tok;
		str += tok.size;
		remaining -= tok.size;
	}
	return (true);
}
