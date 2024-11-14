/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_find_next_logical_token.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:00:13 by pleander          #+#    #+#             */
/*   Updated: 2024/11/13 10:04:50 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenize.h"
#include "ast.h"

/**
 * @brief Checks if the token is a logical token
 *
 * @param x token to check
 * @return 1 if token is a logical token, 0 otherwise
 */
static int	is_logical_token(t_token x)
{
	if (x.type == TOK_LOGICAL_OR || x.type == TOK_LOGICAL_AND)
		return (1);
	return (0);
}

/**
 * @brief Finds the next logical token. Does not search inside parentheses.
 *
 * @param xs token array
 * @param range search range
 * @return index of next logical token
 */
int	find_next_logical_token(t_token *xs, size_t range[2])
{
	size_t	i;
	int		open_paren;

	open_paren = 0;
	i = range[0];
	while (i < range[1])
	{
		if (is_parenthesis(xs[i]))
		{
			if (xs[i].type == TOK_OPEN_PAREN)
				open_paren++;
			if (xs[i].type == TOK_CLOSE_PAREN)
				open_paren--;
		}
		if (open_paren == 0)
		{
			if (is_logical_token(xs[i]))
				return (i);
		}
		i++;
	}
	if (open_paren != 0)
		return (-1);
	return (find_next_token_of_type(xs, TOK_END, range));
}
