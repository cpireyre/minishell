/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:29:23 by copireyr          #+#    #+#             */
/*   Updated: 2024/09/30 13:13:39 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

#include <stdlib.h>
#include <limits.h>
#include "libft.h"

enum e_type
{
	WORD,
	APPEND,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	LOGICAL_AND,
	LOGICAL_OR,
	PIPE,
	META,
	TOKENIZE_SPACE,
	ERROR,
	END,
	NUM_TYPES,
};

typedef struct s_token
{
	const char	*data;
	size_t		size;
	enum e_type	type;
}	t_token;

t_token	*tokenize(const char *str);
void	tokenize_show(t_token token);
void	tokenize_show_tokens(t_token *xs);
void	tokenize_test(void);

void	parse(t_token *xs);

#endif /* TOKENIZE_H */
