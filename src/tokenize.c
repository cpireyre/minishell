/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 12:46:17 by copireyr          #+#    #+#             */
/*   Updated: 2024/09/16 11:45:07 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "tokenize.h"

void	token_show(t_token token);
t_token	token_next(char *input, size_t	start_offset, size_t input_size);
enum e_token_types	token_type(t_token token);

void	tokenize(char *input)
{
	size_t	input_size;
	size_t	offset;
	t_token	token;

	if (!input)
		return ;
	input_size = ft_strlen(input);
	offset = 0;
	while (offset < input_size)
	{
		token = token_next(input, offset, input_size);
		offset += token.size;
		token_show(token);
	}
	ft_dprintf(2, "input size: %u\n", input_size);
}

t_token	token_next(char *input, size_t	start_offset, size_t input_size)
{
	t_token result;
	size_t	i;

	result = (t_token){0};
	i = start_offset;
	if (i < input_size)
	{
		if (ft_isspace(input[i]))
			while (i < input_size && ft_isspace(input[i]))
				i++;
		else
			while (i < input_size && !ft_isspace(input[i]))
				i++;
	result.size = i - start_offset;
	result.data = input + start_offset;
	result.type = token_type(result);
	}
	return (result);
}

enum e_token_types	token_type(t_token token)
{
	if (token.size == 1)
	{
		if (*token.data == '|')
			return (PIPE);
		if (*token.data == '<')
			return (REDIRECT_IN);
		if (*token.data == '>')
			return (REDIRECT_OUT);
	}
	return (NULL_TOKEN);
}

void	token_show(t_token token)
{
	static char	*types[NUM_TOKENS] = {"SYMBOL",
	"SINGLE_QUOTE_STRING",
	"DOUBLE_QUOTE_STRING",
	"REDIRECT_IN",
	"REDIRECT_OUT",
	"REDIRECT_APPEND",
	"HEREDOC_START",
	"PIPE",
	"VAR",
	"NULL_TOKEN",
	};
	ft_dprintf(2, "Token contents: [");
	write(2, token.data, token.size);
	ft_dprintf(2, "]\n");
	ft_dprintf(2, "Token type: %s\n", types[token.type]);
	ft_dprintf(2, "Token size: %u\n", token.size);
	ft_dprintf(2, "\n---END_TOKEN---\n");
}
