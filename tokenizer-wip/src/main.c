/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 09:49:15 by copireyr          #+#    #+#             */
/*   Updated: 2024/09/24 12:24:50 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum e_type
{
	META,
	WORD,
	PIPE,
	LOGICAL_OR,
	LOGICAL_AND,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
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

int	is_meta_character(char c)
{
	return (c == '<' || c == '>'
		|| c == '&' || c == '|');
}

enum e_type	token_meta_get_type(t_token meta)
{
	if (meta.size == 2)
	{
		if (!memcmp(meta.data, "||",  meta.size))
			return (LOGICAL_OR);
		else if (!memcmp(meta.data, "&&",  meta.size))
			return (LOGICAL_AND);
		else if (!memcmp(meta.data, "<<",  meta.size))
			return (HEREDOC);
		else if (!memcmp(meta.data, ">>",  meta.size))
			return (APPEND);
	}
	else if (meta.size == 1)
	{
		if (*meta.data == '>')
			return (REDIRECT_OUT);
		else if (*meta.data == '<')
			return (REDIRECT_IN);
		else if (*meta.data == '|')
			return (PIPE);
	}
	return (ERROR);
}

t_token	*tokenize(const char *str)
{
	const static char	*token_types[NUM_TYPES] = {"Meta", "Word", "Pipe", "Logical OR", "Logical AND", "Redirect IN", "Redirect OUT", "Append", "Heredoc", "Error"};
	t_token	token;
	size_t	len;
	size_t	i;
	t_token	*tokens;
	size_t	capacity;
	size_t	token_count;

	len = strlen(str);
	capacity = 16;
	tokens = malloc(sizeof(*tokens) * capacity);
	if (!tokens)
		return (NULL);
	i = 0;
	token_count = 0;
	while (i < len)
	{
		while (i < len && isspace(str[i]))
			i++;
		if (i == len)
		{
			tokens[token_count++].type = END;
			return (tokens);
		}
		token.data = str + i;
		if (is_meta_character(str[i]))
		{
			token.type = META;
			while (is_meta_character(str[i]))
				i++;
		}
		else
		{
			while (i < len && !isspace(str[i]) && !is_meta_character(str[i]))
			{
				token.type = WORD;
				if (str[i] == '"')
				{
					size_t	j = i + 1;
					while  (j < len && str[j] != '"')
						j++;
					if (str[j] != '"')
						token.type = ERROR;
					i = j;
				}
				if (str[i] == '\'')
				{
					size_t	j = i + 1;
					while  (j < len && str[j] != '\'')
						j++;
					if (str[j] != '\'')
						token.type = ERROR;
					i = j;
				}
				i++;
			}
		}
		token.size = str + i - token.data;
		if (token.type == META)
			token.type = token_meta_get_type(token);
		tokens[token_count++] = token;
	}
	printf("\n");
	return (tokens);
}

int	main(int argc, char **argv)
{
	const char	*str = argv[1];
	t_token	*tokens = tokenize(str);
	size_t	i;

	j = 0;
	while (tokens[j].type != END)
	{
		t_token token = tokens[curr];
		printf("%s token, length %zu: ", token_types[token.type], token.size);
		for (size_t	i = 0; i < token.size; i++)
			putchar(token.data[i]);
		putchar('\n');
	}
}
