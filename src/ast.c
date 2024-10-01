/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:07:01 by pleander          #+#    #+#             */
/*   Updated: 2024/10/01 17:04:40 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "arena.h"
#include "get_next_line.h"
#include "libft.h"
#include "tokenize.h"

// typedef struct	s_ast_root
// {
// 	enum e_ast_type	type;
// 	void			*node;
// }	t_ast_node;
/**
 * @brief Calculates the amount of tokens of type in the token array
 *
 * @param xs token array
 * @param type type of token
 * @return amount of tokens of type
 */
static size_t	count_tokens(t_token *xs, enum e_tok_type type)
{
	size_t	i;
	size_t	n;

	n = 0;
	i = 0;
	while (xs[i].type != TOK_END)
	{
		if (xs[i].type == type)
			n++;
		i++;
	}
	return (n);
}

static size_t	count_logicals(t_token *xs)
{
	size_t	tokens;

	tokens = 0;
	tokens += count_tokens(xs, TOK_LOGICAL_AND);
	tokens += count_tokens(xs, TOK_LOGICAL_OR);
	return (tokens);
}

static enum e_ast_type get_highest_ast_type(t_token *xs)
{
	if (count_tokens(xs, TOK_LOGICAL_OR) +
		count_tokens(xs, TOK_LOGICAL_AND) > 0)
		return (AST_LOGICAL);
	else if (count_tokens(xs, TOK_PIPE) > 0)
		return (AST_PIPELINE);
	else 
		return (AST_COMMAND);
}

static int	find_next_token_of_type(t_token *xs, enum e_tok_type type)
{
	int	i;

	i = 0;
	while (xs[i].type != TOK_END)
	{
		if (xs[i].type == type)
			return (i);
		i++;
	}
	return (-1);
}

static int	find_next_logical_token(t_token *xs)
{
	int	or_ind;
	int	and_ind;

	or_ind = find_next_token_of_type(xs, TOK_LOGICAL_OR);
	and_ind = find_next_token_of_type(xs, TOK_LOGICAL_AND);
	if (or_ind < 0 && and_ind < 0)
		return (-1);
	if (or_ind < 0 && and_ind >= 0)
		return (and_ind);
	if (and_ind < 0 && or_ind >= 0)
		return (or_ind);
	if (and_ind > or_ind)
		return (and_ind);
	else
		return (or_ind);
}

static void show_token_range(t_token *xs, size_t range[2])
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
t_ast_node	*create_ast(t_token *xs, t_ast_node *ast, size_t range[2], t_arena arena)
{
	enum e_ast_type max_type;
	size_t	new_range[2];

	max_type = get_highest_ast_type(xs);
	if (max_type == AST_LOGICAL)
	{
		int ltoken = find_next_logical_token(xs);
		if (ltoken < 0)
			return (NULL);
		new_range[0] = range[0];
		new_range[1] = (size_t)ltoken;
		show_token_range(xs, range);
		show_token_range(xs, new_range);
	}
	return (NULL);
}
