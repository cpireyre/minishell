/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:51:54 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/04 14:01:54 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenize.h"

static enum e_tok_type	token_get_type(char c);
static t_token			*realloc_token_vector_if_needed(t_arena arena,
							t_token *xs, size_t *capacity, size_t count);
static enum e_tok_type	get_operator(t_token token);
static t_token			token_next(const char *str);

t_token	*tokenize(t_arena arena, const char *str)
{
	t_token			*xs;
	const char		*ptr = str;
	const long		len = ft_strlen(str);
	size_t			i;
	size_t			capacity;

	i = 0;
	capacity = 0;
	xs = NULL;
	while (str - ptr <= len && i < INT_MAX)
	{
		xs = realloc_token_vector_if_needed(arena, xs, &capacity, i);
		if (!xs)
			return (NULL);
		while (ft_isspace(*str))
			str++;
		xs[i] = token_next(str);
		if (xs[i].type == TOK_META)
			xs[i].type = get_operator(xs[i]);
		str += xs[i].size;
		if (xs[i++].type == TOK_END)
			break ;
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

static t_token	token_next(const char *str)
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
	return (result);
}

static enum e_tok_type	get_operator(t_token token)
{
	if (token.type != TOK_META)
		return (token.type);
	else if (token.size == 2 && !ft_memcmp(token.value, ">>", 2))
		return (TOK_APPEND);
	else if (token.size == 2 && !ft_memcmp(token.value, "<<", 2))
		return (TOK_HEREDOC);
	else if (token.size == 2 && !ft_memcmp(token.value, "||", 2))
		return (TOK_LOGICAL_OR);
	else if (token.size == 2 && !ft_memcmp(token.value, "&&", 2))
		return (TOK_LOGICAL_AND);
	else if (token.size == 1 && !ft_memcmp(token.value, ">", 1))
		return (TOK_REDIRECT_OUT);
	else if (token.size == 1 && !ft_memcmp(token.value, "<", 1))
		return (TOK_REDIRECT_IN);
	else if (token.size == 1 && !ft_memcmp(token.value, "|", 1))
		return (TOK_PIPE);
	else if (token.size == 1 && !ft_memcmp(token.value, "(", 1))
		return (TOK_OPEN_PAREN);
	else if (token.size == 1 && !ft_memcmp(token.value, ")", 1))
		return (TOK_CLOSE_PAREN);
	else
		return (TOK_ERROR);
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
