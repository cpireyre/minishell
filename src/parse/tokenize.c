/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:51:54 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/14 10:27:41 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenize.h"

static enum e_tok_type	token_get_type(char c);
static t_token			*realloc_token_vector_if_needed(t_arena arena,
							t_token *xs, size_t *capacity, size_t count);
static bool				get_operator(t_arena arena, t_token input,
							t_token **tokens, size_t *capacity, size_t *count);
static t_token			token_next(t_arena arena, const char *str);

t_token	*tokenize(t_arena arena, const char *str)
{
	t_token			*xs;
	const char		*ptr = str;
	const long		len = ft_strlen(str);
	size_t			i;
	size_t			capacity;
	t_token			next;

	i = 0;
	capacity = 0;
	xs = NULL;
	while (str - ptr <= len && i < INT_MAX)
	{
		while (ft_isspace(*str))
			str++;
		next = token_next(arena, str);
		if (next.type == TOK_META)
		{
			if (!get_operator(arena, next, &xs, &capacity, &i))
				return (NULL);
		}
		else
		{
			xs = realloc_token_vector_if_needed(arena, xs, &capacity, i);
			if (!xs)
				return (NULL);
			xs[i++] = next;
			str += next.size;
		}
		if (next.type == TOK_END)
			break ;
		str += next.size;
	}
	return (xs);
}

static t_token	*realloc_token_vector_if_needed(t_arena arena, t_token *xs,
		size_t *capacity, size_t count)
{
	t_token		*tmp;

	if (count < *capacity)
		return (xs);
	*capacity = 2 * *capacity + 1;
	tmp = arena_alloc(arena, sizeof(t_token) * *capacity);
	if (!tmp)
		return (NULL);
	if (xs)
		ft_memcpy(tmp, xs, sizeof(*xs) * count);
	return (tmp);
}

static t_token	token_next(t_arena arena, const char *str)
{
	t_token	result;

	result = (t_token){.value = str, .type = token_get_type(*str), .size = 0};
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
	{"||",	2, TOK_LOGICAL_OR},
	{">>",	2, TOK_APPEND},
	{"<<",	2, TOK_HEREDOC},
	{"&&",	2, TOK_LOGICAL_AND},
	{">",	1, TOK_REDIRECT_OUT},
	{"<",	1, TOK_REDIRECT_IN},
	{"|",	1, TOK_PIPE},
	{"(",	1, TOK_OPEN_PAREN},
	{")",	1, TOK_CLOSE_PAREN},
	{NULL, 0, TOK_ERROR}
	};
	int								i;

	i = 0;
	while (operators[i].str)
	{
		if (remaining >= operators[i].len
		&& !ft_strncmp(str, operators[i].str, operators[i].len))
			return ((t_token){.value = ft_arena_strndup(arena, str, operators[i].len), .size = operators[i].len, .type = operators[i].type});
		i++;
	}
	return ((t_token){.type = TOK_ERROR});
}

static bool	get_operator(t_arena arena, t_token input,
							t_token **tokens, size_t *capacity, size_t *count)
{
	const char	*str = input.value;
	size_t		remaining;
	t_token		tok;

	remaining = input.size;
	while (remaining > 0)
	{
		*tokens = realloc_token_vector_if_needed(arena, *tokens, capacity, *count);
		if (!*tokens)
			return (false);
		tok = match_operator(arena, str, remaining);
		if (tok.type == TOK_ERROR)
			return (false);
		(*tokens)[(*count)++] = tok;
		str += tok.size;
		remaining -= tok.size;
	}
	return (true);
}

static enum e_tok_type	token_get_type(char c)
{
	if (c == '<' || c == '>'
		|| c == '&' || c == '|'
		|| c == '(' || c == ')')
		return (TOK_META);
	else if (ft_isspace(c))
		return (TOK_TOKENIZE_SPACE);
	else if (c == '\0')
		return (TOK_END);
	else
		return (TOK_WORD);
}
