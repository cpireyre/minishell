/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_create_command_node.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:55:50 by pleander          #+#    #+#             */
/*   Updated: 2024/11/20 10:25:04 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

static size_t	count_redirs(t_token *xs, size_t range[2])
{
	size_t	sum;

	sum = 0;
	sum += count_tokens(xs, TOK_REDIRECT_IN, range);
	sum += count_tokens(xs, TOK_REDIRECT_OUT, range);
	sum += count_tokens(xs, TOK_HEREDOC, range);
	return (sum);
}

static	char	*concat_token_values(t_token *xs, size_t range[2],
				t_arena arena)
{
	char	*str;
	size_t	len;
	size_t	i;

	len = 0;
	i = range[0];
	while (i < range[1])
	{
		len += xs[i].size + 1;
		i++;
	}
	str = arena_calloc(arena, len, sizeof(char));
	if (!str)
		return (NULL);
	i = range[0];
	while (i < range[1])
	{
		ft_strlcat(str, xs[i].value, len * sizeof(char));
		if (i + 1 < range[1])
			ft_strlcat(str, " ", len * sizeof(char));
		i++;
	}
	return (str);
}

static t_ast_node	*init_cmd_node(t_token *xs, size_t range[2], t_arena arena)
{
	t_ast_node	*cmd_node;
	size_t		word_count;

	cmd_node = arena_alloc(arena, sizeof(t_ast_node));
	if (!cmd_node)
		return (NULL);
	word_count = count_tokens(xs, TOK_WORD, range);
	if (word_count < count_redirs(xs, range))
	{
		syntax_error();
		return (NULL);
	}
	cmd_node->type = AST_COMMAND;
	cmd_node->token.value = concat_token_values(xs, range, arena);
	cmd_node->token.size = ft_strlen(cmd_node->token.value);
	cmd_node->token.type = TOK_COMMAND;
	cmd_node->n_children = word_count;
	cmd_node->children = arena_calloc(arena, cmd_node->n_children + 1,
			sizeof(t_ast_node *));
	if (!cmd_node->children)
		return (NULL);
	return (cmd_node);
}

static t_ast_node	*create_redir_node(t_ast_node *redir_node, t_token *xs,
		size_t range[2], t_arena arena)
{
	redir_node->type = AST_REDIR;
	if (xs[range[0] + 1].type != TOK_WORD)
	{
		syntax_error();
		return (NULL);
	}
	if (range[0] + 1 < range[1] && xs[range[0]].type != TOK_END)
	{
		redir_node->children = arena_calloc(arena, 1, sizeof(t_ast_node *));
		if (!redir_node->children)
			return (NULL);
		redir_node->children[0] = arena_calloc(arena, 1, sizeof(t_ast_node));
		redir_node->children[0]->type = AST_WORD;
		redir_node->children[0]->token = xs[range[0] + 1];
		redir_node->children[0]->n_children = 0;
		redir_node->n_children = 1;
		range[0]++;
	}
	else
	{
		syntax_error();
		return (NULL);
	}
	return (redir_node);
}

/**
 * @brief Creates a command ast and all words or redirects as children
 * of that node
 *
 * @param xs: token array
 * @param range: range to operate in token array
 * @param arena: the memory arena
 * @return the command ast node
 */
t_ast_node	*create_command_node(t_token *xs, size_t range[2], t_arena arena)
{
	t_ast_node	*cmd_node;
	int			i;

	cmd_node = init_cmd_node(xs, range, arena);
	if (!cmd_node)
		return (NULL);
	i = 0;
	while (range[0] < range[1] && xs[range[0]].type != TOK_END)
	{
		cmd_node->children[i] = arena_calloc(arena, 1, sizeof(t_ast_node));
		if (!cmd_node->children[i])
			return (NULL);
		cmd_node->children[i]->token = xs[range[0]];
		if (xs[range[0]].type == TOK_WORD)
			cmd_node->children[i]->type = AST_WORD;
		else
		{
			if (create_redir_node(cmd_node->children[i],
					xs, range, arena) == NULL)
				return (NULL);
		}
		range[0]++;
		i++;
	}
	return (cmd_node);
}
