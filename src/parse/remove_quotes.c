/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 09:48:17 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/13 13:14:41 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

char	*remove_quotes_from_str(t_arena arena, const char *str)
{
	char	*result;
	char	*save;
	char	quote;

	if (!ft_strchr(str, '"') && !ft_strchr(str, '\''))
		return ((char *)str);
	result = arena_alloc(arena, ft_strlen(str) + 1);
	if (!result)
		return ((char *)str);
	save = result;
	while (*str)
	{
		if (*str != '"' && *str != '\'')
			*result++ = *str++;
		else if (*str == '"' || *str == '\'')
		{
			quote = *str++;
			while (*str && *str != quote)
				*result++ = *str++;
		}
	}
	*result = '\0';
	return (save);
}

/* TODO: Check return value for ENOMEM */
void	remove_quotes(t_arena arena, t_ast_node *ast)
{
	size_t		i;
	const char	*str;

	if (!ast)
		return ;
	i = 0;
	while (i < ast->n_children)
	{
		if (ast->children[i]->type == AST_WORD
			&& ast->children[i]->token.is_globbed == false)
		{
			str = remove_quotes_from_str(arena, ast->children[i]->token.value);
			ast->children[i]->token.value = str;
			ast->children[i]->token.size = ft_strlen(str);
		}
		remove_quotes(arena, ast->children[i]);
		i++;
	}
}
