/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:13:38 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/01 10:42:54 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenize.h"

const char	*ast_show_type(enum e_type type)
{
	static const char	*types[AST_NUM_TYPES] = {
		"AST_WORD",
		"AST_COMMAND",
		"AST_APPEND",
		"AST_REDIRECT_IN",
		"AST_REDIRECT_OUT",
		"AST_HEREDOC",
		"AST_LOGICAL_AND",
		"AST_LOGICAL_OR",
		"AST_PIPE",
		"AST_ERROR",
		"AST_META",
		"AST_TOKENIZE_SPACE",
		"AST_END",
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
	while (xs[j].type != AST_END)
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

	ft_dprintf(2, "Running tokenizer tests...\n");
	j = 0;
	while (tests[j])
	{
		ft_printf("Tokenizing: %s\n", tests[j]);
		xs = tokenize(tests[j]);
		if (xs)
		{
			tokenize_show_tokens(xs);
			free(xs);
		}
		else
			ft_dprintf(2, "ENOMEM??\n");
		j++;
	}
}
