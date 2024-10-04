/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:13:38 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/04 13:23:09 by copireyr         ###   ########.fr       */
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
	ft_printf("\n");
}

void	tokenize_test(void)
{
	const char	*tests[] = {"ls", "echo a", "date > outfile",
		"| ah &&", "echo\"a   b   c\"", "\"a\"", "\"a",
		"echo\"a b c \"'this should be one though   'still   tok",
		"cat << EOF hello heredoc >>",
		"cat < infile > outfile",
		NULL};
	t_token		*xs;
	size_t		j;
	t_arena		arena;

	ft_dprintf(2, "Running tokenizer tests...\n");
	j = 0;
	while (tests[j])
	{
		arena = arena_new();
		if (arena)
		{
			ft_printf("Tokenizing: %s\n", tests[j]);
			xs = tokenize(arena, tests[j]);
			if (xs)
			{
				tokenize_show_tokens(xs);
				free(xs);
			}
			else
				ft_dprintf(2, "ENOMEM??\n");
			arena_dispose(&arena);
		}
		j++;
	}
}
