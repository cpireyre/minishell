/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:29:23 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/12 09:44:19 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

# include "libft.h"
# include "arena.h"
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

# define UNQUOTED 0
# define DOUBLE_QUOTED 1
# define SINGLE_QUOTED 2

typedef struct s_char_meta
{
	uint8_t	quote_type : 2;
	uint8_t	from_expansion : 1;
	uint8_t	from_glob : 1;
	uint8_t	unused : 4;
}	t_char_meta;

typedef struct s_meta_string
{
	char		*str;
	t_char_meta	*meta;
}	t_meta_string;

typedef struct s_token
{
	enum e_tok_type	type;
	char			*value;
	t_char_meta		*meta;
	size_t			size;
	bool			is_globbed;
	int				num_expandables;
}	t_token;

typedef struct s_token_vector
{
	t_token	*data;
	int		len;
	int		capacity;
}	t_token_vec;

struct s_operator
{
	const char			*str;
	size_t				len;
	enum e_tok_type		type;
};

extern t_token		*tokenize(t_arena arena, char *str);
void				tokenize_show(t_token token);
void				tokenize_show_tokens(t_token *xs);
size_t				count_toks(t_token *xs);
const char			*ast_show_type(enum e_tok_type type);
enum 				e_tok_type	token_get_type(char c);

#endif /* TOKENIZE_H */
