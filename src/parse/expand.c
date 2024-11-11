/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 10:00:45 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/11 09:05:51 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "libft.h"
#include "quotes.h"

/* We start max_var_len at 11 so we can print any integer exit code */
size_t	get_max_env_var_len(t_list *env)
{
	size_t	max_var_len;
	size_t	curr_var_len;

	max_var_len = 11;
	while (env)
	{
		curr_var_len = ft_strlen(env->content);
		if (curr_var_len > max_var_len)
			max_var_len = curr_var_len;
		env = env->next;
	}
	return (max_var_len);
}

size_t	calculate_scratch_space(t_token *xs, t_list *env)
{
	const size_t	max_var_len = get_max_env_var_len(env);
	t_quote			*str;
	size_t			expansion_size;
	size_t			max_expansion_size;

	max_expansion_size = 0;
	while (xs->type != TOK_END)
	{
		str = xs->q_value;
		xs->size = 0;
		while (str[xs->size])
		{
			if ((str[xs->size] & CHAR_MASK) == '$'
				&& is_quoted(str[xs->size]) != '\'')
				xs->num_expandables++;
			xs->size++;
		}
		expansion_size = (xs->size - xs->num_expandables)
			+ (xs->num_expandables * max_var_len);
		if (expansion_size > max_expansion_size)
			max_expansion_size = expansion_size;
		xs++;
	}
	return (sizeof(t_quote) * (max_expansion_size + 1));
}

const char	*get_expansion(t_quote *str, t_list *env, const char *exit_code)
{
	char	*entry;
	int		i;

	if (!str[0])
		return ("$");
	if ((str[0] & CHAR_MASK) == '?')
		return (exit_code);
	if (ft_isalpha(str[0] & CHAR_MASK) || (str[0] & CHAR_MASK) == '_')
	{
		while (env)
		{
			entry = env->content;
			i = 0;
			while (entry[i] && entry[i] != '=' && ft_isword(str[i] & CHAR_MASK)
				&& (str[i] & CHAR_MASK) == entry[i])
				i++;
			if (entry[i] == '=' && (!str[i] || !ft_isword(str[i] & CHAR_MASK)))
				return (entry + i + 1);
			env = env->next;
		}
		return ("");
	}
	return ("$");
}

size_t	expand_token(t_quote *dst, t_quote *src, t_list *env, const char *code)
{
	size_t		j;
	char		quote;
	const char	*expansion;

	j = 0;
	while (*src)
	{
		if ((*src & CHAR_MASK) == '$' && is_quoted(*src) != '\'')
		{
			quote = *src++ & QUOTE_MASK;
			expansion = get_expansion(src, env, code);
			while (*expansion)
				dst[j++] = *expansion++ | quote;
			if ((*src & CHAR_MASK) == '?')
				src++;
			else
				while (*src && ft_isword(*src & CHAR_MASK))
					src++;
		}
		else
			dst[j++] = *src++;
	}
	dst[j] = 0;
	return (j + 1);
}

t_token	*expand(t_arena arena, t_token *xs, t_list *env, int exit_code)
{
	const size_t	scratch_size = calculate_scratch_space(xs, env);
	t_quote			*scratch;
	char			exit_buf[12];
	int				expanded_size;

	scratch = malloc(scratch_size);
	if (!scratch)
		return (NULL);
	ft_snprintf(exit_buf, sizeof(exit_buf), "%d", exit_code);
	while (xs->type != TOK_END)
	{
		if (xs->num_expandables)
		{
			ft_bzero(scratch, scratch_size);
			expanded_size = expand_token(scratch, xs->q_value, env, exit_buf);
			xs->q_value = arena_calloc(arena, expanded_size, sizeof(t_quote));
			if (!xs->q_value)
				return (NULL);
			ft_memcpy(xs->q_value, scratch, expanded_size * sizeof(t_quote));
		}
		if (DEBUG)
			ft_printf("Expanded: %s\n", quotes_lower(arena, xs->q_value));
		xs++;
	}
	return (xs);
}
