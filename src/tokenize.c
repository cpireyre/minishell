/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:51:54 by copireyr          #+#    #+#             */
/*   Updated: 2024/09/26 15:23:12 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenize.h"

static enum e_type	token_get_type(char c);
static t_token		*realloc_token_vector_if_needed(t_token *xs,
						size_t *capacity, size_t count);
static enum e_type	get_operator(t_token token);
static t_token		token_next(const char *str);

t_token	*tokenize(const char *str)
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
		xs = realloc_token_vector_if_needed(xs, &capacity, i);
		if (!xs)
			return (NULL);
		while (ft_isspace(*str))
			str++;
		xs[i] = token_next(str);
		if (xs[i].type == META)
			xs[i].type = get_operator(xs[i]);
		str += xs[i].size;
		if (xs[i++].type == END)
			break ;
	}
	return (xs);
}

static t_token	*realloc_token_vector_if_needed(t_token *xs,
		size_t *capacity, size_t count)
{
	t_token		*tmp;

	if (count < *capacity)
		return (xs);
	*capacity = 2 * *capacity + 1;
	tmp = malloc(sizeof(t_token) * *capacity);
	if (!tmp)
	{
		free(xs);
		return (NULL);
	}
	if (xs)
	{
		ft_memcpy(tmp, xs, sizeof(*xs) * count);
		free(xs);
	}
	return (tmp);
}

static t_token	token_next(const char *str)
{
	t_token	result;

	result = (t_token){.data = str, .type = token_get_type(*str), .size = 0};
	while (result.type != END && token_get_type(*str) == result.type)
	{
		if (result.type == WORD && *str == '"')
		{
			str = ft_strchrnul(str + 1, '"');
			if (*str != '"')
				result.type = ERROR;
		}
		if (result.type == WORD && *str == '\'')
		{
			str = ft_strchrnul(str + 1, '\'');
			if (*str != '\'')
				result.type = ERROR;
		}
		if (result.type != ERROR)
			str++;
	}
	result.size = str - result.data;
	return (result);
}

static enum e_type	get_operator(t_token token)
{
	if (token.type != META)
		return (token.type);
	else if (token.size == 2 && !ft_memcmp(token.data, ">>", 2))
		return (APPEND);
	else if (token.size == 2 && !ft_memcmp(token.data, "<<", 2))
		return (HEREDOC);
	else if (token.size == 2 && !ft_memcmp(token.data, "||", 2))
		return (LOGICAL_OR);
	else if (token.size == 2 && !ft_memcmp(token.data, "&&", 2))
		return (LOGICAL_AND);
	else if (token.size == 1 && !ft_memcmp(token.data, ">", 1))
		return (REDIRECT_OUT);
	else if (token.size == 1 && !ft_memcmp(token.data, "<", 1))
		return (REDIRECT_IN);
	else if (token.size == 1 && !ft_memcmp(token.data, "|", 1))
		return (PIPE);
	else
		return (ERROR);
}

static enum e_type	token_get_type(char c)
{
	if (c == '<' || c == '>' || c == '&' || c == '|')
		return (META);
	else if (ft_isspace(c))
		return (TOKENIZE_SPACE);
	else if (c == '\0')
		return (END);
	else
		return (WORD);
}
