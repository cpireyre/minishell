/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 09:51:57 by copireyr          #+#    #+#             */
/*   Updated: 2024/09/26 14:23:31 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

enum e_type
{
	WORD,
	APPEND,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	LOGICAL_AND,
	LOGICAL_OR,
	PIPE,
	META,
	SPACE,
	ERROR,
	END,
	NUM_TYPES,
};

typedef struct s_token
{
	const char	*data;
	size_t		size;
	enum e_type	type;
}	t_token;

enum e_type	token_get_type(char c)
{
	if (c == '<' || c == '>' || c == '&' || c == '|')
		return (META);
	else if (isspace(c))
		return (SPACE);
	else if (c == '\0')
		return (END);
	else
		return (WORD);
}

const char	*ft_strchrnul(const char *str, int c)
{
	while (*str && *str != c)
		str++;
	return (str);
}

t_token	token_next(const char *str)
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

t_token	*realloc_token_vector_if_needed(t_token *xs,
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
		memcpy(tmp, xs, sizeof(*xs) * count);
		free(xs);
	}
	return (tmp);
}

enum e_type	token_meta_get_type(t_token token)
{
	if (token.type != META)
		return (token.type);
	else if (token.size == 2 && !memcmp(token.data, ">>", 2))
		return (APPEND);
	else if (token.size == 2 && !memcmp(token.data, "<<", 2))
		return (HEREDOC);
	else if (token.size == 2 && !memcmp(token.data, "||", 2))
		return (LOGICAL_OR);
	else if (token.size == 2 && !memcmp(token.data, "&&", 2))
		return (LOGICAL_AND);
	else if (token.size == 1 && !memcmp(token.data, ">", 1))
		return (REDIRECT_OUT);
	else if (token.size == 1 && !memcmp(token.data, "<", 1))
		return (REDIRECT_IN);
	else if (token.size == 1 && !memcmp(token.data, "|", 1))
		return (PIPE);
	else
		return (ERROR);
}

t_token	*tokenize(const char *str)
{
	t_token			*xs;
	const char		*ptr = str;
	const size_t	len = strlen(str);
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
		while (isspace(*str))
			str++;
		xs[i] = token_next(str);
		if (xs[i].type == META)
			xs[i].type = token_meta_get_type(xs[i]);
		str += xs[i].size;
		if (xs[i++].type == END)
			break ;
	}
	return (xs);
}

void	token_show(t_token token)
{
	static const char	*types[NUM_TYPES] = {
		"WORD",
		"APPEND",
		"REDIRECT_IN",
		"REDIRECT_OUT",
		"HEREDOC",
		"LOGICAL_AND",
		"LOGICAL_OR",
		"PIPE",
		"META",
		"SPACE",
		"ERROR",
		"END",
	};
	size_t				i;

	putchar('[');
	i = 0;
	while (i < token.size)
		putchar(token.data[i++]);
	printf(" (%s)] ", types[token.type]);
}

int	main(int argc, char **argv)
{
	t_token	*xs;
	size_t	i;

	if (argc > 1)
	{
		xs = tokenize(argv[1]);
		if (xs)
		{
			i = 0;
			while (xs[i].type != END)
				token_show(xs[i++]);
			puts("");
			free(xs);
		}
		else
			printf("ENOMEM??\n");
	}
}
