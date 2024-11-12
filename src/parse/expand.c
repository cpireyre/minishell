/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 10:00:45 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/12 10:23:24 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "libft.h"

/* We start max_var_len at 11 so we can print any integer exit code,
 * even if there aren't any environment variables */
size_t	calculate_scratch_space(t_token *xs, t_list *env)
{
	size_t			max_var_len;
	size_t			max_expansion_len;
	size_t			curr_len;
	int				i;

	max_var_len = 11;
	while (env)
	{
		curr_len = ft_strlen(env->content);
		if (curr_len > max_var_len)
			max_var_len = curr_len;
		env = env->next;
	}
	max_expansion_len = max_var_len;
	while (xs->type != TOK_END)
	{
		curr_len = ft_strlen(xs->value);
		i = -1;
		while (xs->value[++i])
			if (xs->value[i] == '$' && xs->meta[i].quote_type != SINGLE_QUOTED)
				curr_len += max_var_len;
		if (curr_len > max_expansion_len)
			max_expansion_len = curr_len;
		xs++;
	}
	return (max_expansion_len);
}

t_token	*expand(t_arena arena, t_token *xs, t_list *env, int exit_code)
{
	const size_t	scratch_size = calculate_scratch_space(xs, env);
	char			*scratch;
	int				i;
	char			code[12];

	ft_snprintf(code, 12, "%d", exit_code);
	scratch = arena_alloc(arena, scratch_size * (sizeof(char) + sizeof(t_char_meta)));
	if (!scratch)
		return (NULL);
	i = 0;
	while (xs[i].type != TOK_END)
	{
		if (xs[i].type == TOK_WORD)
		{
			ft_bzero
		}
		i++;
	}
	return (xs);
}
