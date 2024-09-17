/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 12:46:16 by copireyr          #+#    #+#             */
/*   Updated: 2024/09/16 10:58:59 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

enum e_token_types
{
	SYMBOL,
	SINGLE_QUOTE_STRING,
	DOUBLE_QUOTE_STRING,
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	HEREDOC_START,
	PIPE,
	VAR,
	NULL_TOKEN,
	NUM_TOKENS,
};

typedef struct s_token
{
	char				*data;
	size_t				size;
	enum e_token_types	type;
}	t_token;

void	tokenize(char *str);

#endif /* TOKENIZE_H */
