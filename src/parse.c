/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:57:35 by copireyr          #+#    #+#             */
/*   Updated: 2024/09/30 13:19:36 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenize.h"

enum e_ast_type
{
	COMMAND,
	ARGUMENT,
	AST_NUM_TYPES,
};

typedef struct s_ast
{
	enum e_ast_type	type;
	const char		*value;
	size_t			value_size;
	struct s_ast	*nodes;
}	t_ast;

int		has_error_token(t_token *xs)
{
	size_t	i;

	i = 0;
	while (xs[i].type != END)
	{
		if (xs[i].type == ERROR)
			return (1);
		i++;
	}
	return (0);
}

void	parse(t_token *xs)
{
	size_t	i;

	if (has_error_token(xs))
	{
		ft_dprintf(2, "This is your error message. Will not parse.\n");
		return ;
	}
	i = 0;
	if (xs[i].type == WORD)
	{
		size_t	words = 1;
		while (xs[words].type == WORD)
			words++;
		t_ast	*children = malloc(sizeof(*children) * (words + 1));
		t_ast	node = (t_ast){.type = COMMAND, .value = xs[i].data,
			.value_size = xs[i].size, .nodes = children};
		(void)children;
		(void)node;
		tokenize_show_tokens(xs);
	}
}
