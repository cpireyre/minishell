/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:29:23 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/04 15:10:05 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

# include "libft.h"
# include "arena.h"
# include "tokenize.h"
# include <limits.h>

enum e_tok_type
{
	TOK_WORD,
	TOK_COMMAND,
	TOK_APPEND,
	TOK_REDIRECT_IN,
	TOK_REDIRECT_OUT,
	TOK_HEREDOC,
	TOK_LOGICAL_AND,
	TOK_LOGICAL_OR,
	TOK_PIPE,
	TOK_OPEN_PAREN,
	TOK_CLOSE_PAREN,
	TOK_ERROR,
	TOK_META,
	TOK_TOKENIZE_SPACE,
	TOK_END,
	TOK_NUM_TYPES,
};

/* typedef struct s_ast */
/* { */
/* 	enum e_tok_type		type; */
/* 	char				*value; */
/* 	struct s_ast		**children; */
/* 	size_t				n_children; */
/* }	t_ast; */

typedef struct s_token
{
	enum e_tok_type	type;
	const char		*value;
	size_t			size;
}	t_token;

extern t_token		*tokenize(t_arena arena, const char *str);
void				tokenize_show(t_token token);
void				tokenize_show_tokens(t_token *xs);
size_t				count_toks(t_token *xs);
const char			*ast_show_type(enum e_tok_type type);

#endif /* TOKENIZE_H */
