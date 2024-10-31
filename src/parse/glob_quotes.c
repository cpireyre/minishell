/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:53:50 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/31 13:56:45 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "glob.h"

static bool	should_toggle_quote(char c, char current_quote);

t_quote	*quotes_lift(t_arena arena, const char *str)
{
	t_quote	*result;
	int		j;
	char	current_quote;

	if (!str)
		return (NULL);
	result = arena_calloc(arena, ft_strlen(str) + 1, sizeof(*result));
	if (!result)
		return (NULL);
	j = 0;
	current_quote = 0;
	while (*str)
	{
		if (should_toggle_quote(*str, current_quote))
			current_quote = !current_quote * *str;
		else
			result[j++] = *str | (!!current_quote * QUOTED_BIT);
		str++;
	}
	result[j] = '\0';
	return (result);
}

char	*quotes_lower(t_arena arena, const t_quote *str)
{
	char	*result;
	int		len;

	len = 0;
	while (str[len++])
		;
	result = arena_calloc(arena, len + 1, sizeof(*result));
	while (result && *str)
		*result++ = *str++ & CHAR_MASK;
	*result++ = '\0';
	return (result - len);
}

static bool	should_toggle_quote(char c, char current_quote)
{
	return ((c == '\'' || c == '\"')
		&& (current_quote == 0 || c == current_quote));
}
