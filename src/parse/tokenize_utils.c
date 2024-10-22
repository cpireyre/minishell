/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:13:38 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/04 15:12:00 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenize.h"

const char	*ast_show_type(enum e_tok_type type)
{
	static const char	*types[TOK_NUM_TYPES] = {
		"TOK_WORD",
		"TOK_COMMAND",
		"TOK_APPEND",
		"TOK_REDIRECT_IN",
		"TOK_REDIRECT_OUT",
		"TOK_HEREDOC",
		"TOK_LOGICAL_AND",
		"TOK_LOGICAL_OR",
		"TOK_PIPE",
		"TOK_OPEN_PAREN",
		"TOK_CLOSE_PAREN",
		"TOK_ERROR",
		"TOK_META",
		"TOK_TOKENIZE_SPACE",
		"TOK_END",
	};

	return (types[type]);
}

void	tokenize_show(t_token token)
{
	size_t				i;

	ft_printf("[");
	i = 0;
	while (i < token.size)
		ft_printf("%c", token.value[i++]);
	ft_printf(" (%s)] ", ast_show_type(token.type));
}

void	tokenize_show_tokens(t_token *xs)
{
	size_t	j;

	j = 0;
	while (xs[j].type != TOK_END)
		tokenize_show(xs[j++]);
	tokenize_show(xs[j]);
	ft_printf("\n");
}

size_t	count_toks(t_token *xs)
{
	size_t	i;

	i = 0;
	while (xs[i].type != TOK_END)
		i++;
	i++;
	return (i);
}

enum e_tok_type	token_get_type(char c)
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
