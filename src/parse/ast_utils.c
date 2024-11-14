/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:02:53 by pleander          #+#    #+#             */
/*   Updated: 2024/11/13 10:56:54 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "tokenize.h"

/**
 * @brief Checks if the token x is a parenthesis
 *
 * @param x: token to check
 * @return 1 if true, 0 otherwise
 */
int	is_parenthesis(t_token x)
{
	if (x.type == TOK_OPEN_PAREN || x.type == TOK_CLOSE_PAREN)
		return (1);
	return (0);
}

/**
 * @brief Finds the next token of the type specified within a range of an
 * array of tokens
 *
 * @param xs: full token array
 * @param type: type of token to find
 * @param range: range to check
 * @return index of next token of type type or -1 if it does not exist
 */
int	find_next_token_of_type(t_token *xs, enum e_tok_type type, size_t range[2])
{
	size_t	i;

	i = range[0];
	while (i < range[1])
	{
		if (xs[i].type == type)
			return (i);
		i++;
		if (xs[i].type == TOK_END)
			break ;
	}
	return (-1);
}

/**
 * @brief Calculates the amount of tokens of type in the token array
 *
 * @param xs token array
 * @param type type of token
 * @return amount of tokens of type
 */

size_t	count_tokens(t_token *xs, enum e_tok_type type, size_t range[2])
{
	size_t	i;
	size_t	n;

	n = 0;
	i = range[0];
	while (i < range[1])
	{
		if (xs[i].type == type)
			n++;
		if (xs[i].type == TOK_END)
			break ;
		i++;
	}
	return (n);
}

void	show_token_range(t_token *xs, size_t range[2])
{
	size_t	i;

	i = range[0];
	while (i < range[1])
	{
		tokenize_show(xs[i]);
		i++;
	}
	ft_printf("\n");
}

t_ast_node	*syntax_error(char *msg)
{
	ft_dprintf(2, "Syntax error while %s\n", msg);
	return (NULL);
}
