/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:57:35 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/01 09:12:54 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenize.h"

int	has_error_token(t_token *xs)
{
	size_t	i;

	i = 0;
	while (xs[i].type != AST_END)
	{
		if (xs[i].type == AST_ERROR)
			return (1);
		i++;
	}
	return (0);
}

void	parse(t_token *xs)
{
	(void)xs;
}
