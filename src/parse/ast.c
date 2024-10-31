/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:07:01 by pleander          #+#    #+#             */
/*   Updated: 2024/11/06 12:11:33 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "arena.h"
#include "get_next_line.h"
#include "libft.h"
#include "tokenize.h"

/**
 * @brief Calculates the amount of tokens of type in the token array
 *
 * @param xs token array
 * @param type type of token
 * @return amount of tokens of type
 */
static size_t	count_tokens(t_token *xs, enum e_tok_type type, size_t range[2])
{
	size_t	i;
	size_t	n;

	n = 0;
	i = range[0];
	while (i < range[1])
	{
		if (xs[i].type == type)
			n++;
		if (xs[i].type == TOK_END)
			break ;
		i++;
	}
	return (n);
}

size_t	count_logicals(t_token *xs, size_t range[2])
{
	size_t	tokens;

	tokens = 0;
	tokens += count_tokens(xs, TOK_LOGICAL_AND, range);
	tokens += count_tokens(xs, TOK_LOGICAL_OR, range);
	return (tokens);
}

static int	is_parenthesis(t_token x)
{
	if (x.type == TOK_OPEN_PAREN || x.type == TOK_CLOSE_PAREN)
		return (1);
	return (0);
}

static enum e_ast_type get_highest_ast_type(t_token *xs, size_t range[2])
{
	enum e_ast_type max_ast;
	size_t			i;
	int				open_paren;
	
	open_paren = 0;
	i = range[0];
	max_ast = AST_NONE;
	while (i < range[1])
	{
		if (open_paren == 0)
		{
			if ((xs[i].type == TOK_LOGICAL_OR || xs[i].type == TOK_LOGICAL_AND) && (int)AST_LOGICAL < (int)max_ast)
					max_ast = AST_LOGICAL;
			else if (xs[i].type == TOK_OPEN_PAREN
				&& ((int)max_ast > (int)AST_PAREN))
				max_ast = AST_PAREN;
			else if (xs[i].type == TOK_PIPE
				&& ((int)max_ast > (int)AST_PIPELINE))
					max_ast = AST_PIPELINE;
			else if (xs[i].type == TOK_WORD
				&& ((int)max_ast > (int)AST_COMMAND))
					max_ast = AST_COMMAND;
		}
		if (is_parenthesis(xs[i]))
		{
			if (xs[i].type == TOK_OPEN_PAREN)
				open_paren++;
			else if (xs[i].type == TOK_CLOSE_PAREN)
				open_paren--;
		}
		i++;
	}
	return (max_ast);
}

// static enum e_ast_type get_highest_ast_type(t_token *xs, size_t range[2])
// {
// 	if (count_tokens(xs, TOK_LOGICAL_OR, range) +
// 		count_tokens(xs, TOK_LOGICAL_AND, range) > 0)
// 		return (AST_LOGICAL);
// 	else if (count_tokens(xs, TOK_OPEN_PAREN, range) > 0)
// 			return (AST_PAREN);
// 	else if (count_tokens(xs, TOK_PIPE, range) > 0)
// 		return (AST_PIPELINE);
// 	else 
// 		return (AST_COMMAND);
// }

static int	find_next_token_of_type(t_token *xs, enum e_tok_type type, size_t range[2])
{
	size_t	i;
		
	i = range[0];
	while (i < range[1])
	{
		if (xs[i].type == type)
			return (i);
		i++;
		if (xs[i].type == TOK_END)
			break ;
	}
	return (-1);
}

static int	is_logical_token(t_token x)
{
	if (x.type == TOK_LOGICAL_OR || x.type == TOK_LOGICAL_AND)
		return (1);
	return (0);
}


/**
 * @brief Finds the next logical token. Does not search inside parentheses.
 *
 * @param xs token array
 * @param range search range
 * @return index of next logical token
 */
static int	find_next_logical_token(t_token *xs, size_t range[2])
{
	size_t	i;
	int		open_paren;

	open_paren = 0;
	i = range[0];
	while (i < range[1])
	{
		if (is_parenthesis(xs[i]))
		{
			if (xs[i].type == TOK_OPEN_PAREN)
				open_paren++;
			if (xs[i].type == TOK_CLOSE_PAREN)
				open_paren--;
		}
		if (open_paren == 0)
		{
			if (is_logical_token(xs[i]))
				return (i);
		}
		i++;
	}
	if (open_paren != 0) // Syntax error
		return (-1);
	return (find_next_token_of_type(xs, TOK_END, range));
}

static size_t	count_redirs(t_token *xs, size_t range[2])
{
	size_t	sum;

	sum = 0;
	sum += count_tokens(xs, TOK_REDIRECT_IN, range);
	sum += count_tokens(xs, TOK_REDIRECT_OUT, range);
	sum += count_tokens(xs, TOK_HEREDOC, range);
	return (sum);
}

void show_token_range(t_token *xs, size_t range[2])
{
	size_t	i;

	i = range[0];
	while (i < range[1])
	{
		tokenize_show(xs[i]);
		i++;
	}
	ft_printf("\n");
}

int	add_node_to_parent(t_ast_node *parent, t_ast_node *child, t_arena arena)
{
	t_ast_node	**children;

	if (!parent)
		return (0);
	if (!child)
		return (-1);
	children = arena_alloc(arena, (parent->n_children + 1) * sizeof(t_ast_node *));
	if (!children)
		return (-1);
	if (parent->children)
		ft_memcpy(children, parent->children, parent->n_children * sizeof(t_ast_node *));
	children[parent->n_children] = child;
	parent->children = children;
	parent->n_children++;
	return (0);
}

static	char	*concat_token_values(t_token *xs, size_t range[2], t_arena arena)
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

static	t_ast_node	*syntax_error(char *msg)
{
	ft_dprintf(2, "Syntax error while %s\n", msg);
	return (NULL);
}

// command has first program name, then arguments. If thers's a redirect the following file is outfile
static t_ast_node	*create_command_node(t_token *xs, t_ast_node *parent, size_t range[2], t_arena arena)
{
    (void)parent;
	t_ast_node	*cmd_node;
	size_t		word_count;

	cmd_node = arena_alloc(arena, sizeof(t_ast_node));
	if (!cmd_node)
		return (NULL);
	word_count = count_tokens(xs, TOK_WORD, range);
	cmd_node->type = AST_COMMAND;
	cmd_node->token.value = concat_token_values(xs, range, arena);
	cmd_node->token.size = ft_strlen(cmd_node->token.value);
	cmd_node->token.type = TOK_COMMAND;
	cmd_node->n_children = word_count; // Redirs add a redir but also remove a word
	if (word_count < count_redirs(xs, range))
		return (syntax_error("creating command node"));
	cmd_node->children = arena_calloc(arena, cmd_node->n_children, sizeof(t_ast_node *));
	if (!cmd_node->children)
		return (NULL);
	int i = 0;
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
			cmd_node->children[i]->type = AST_REDIR;
			if (xs[range[0] + 1].type != TOK_WORD)
				return (syntax_error("creating command node"));
			if (range[0] + 1 < range[1] && xs[range[0]].type != TOK_END)
			{
				cmd_node->children[i]->children = arena_calloc(arena, 1, sizeof(t_ast_node *));
				if (!cmd_node->children[i]->children)
					return (NULL);
				cmd_node->children[i]->children[0] = arena_calloc(arena, 1, sizeof(t_ast_node));
				cmd_node->children[i]->children[0]->type = AST_WORD;
				cmd_node->children[i]->children[0]->token = xs[range[0] + 1];
				cmd_node->children[i]->children[0]->n_children = 0;
				cmd_node->children[i]->n_children = 1;
				range[0]++;
			}
			else 
				return (syntax_error("creating command node"));
		}
		range[0]++;
		i++;
	}
	return (cmd_node);
}

t_ast_node	*create_ast(t_token *xs, t_ast_node *parent, size_t range[2], t_arena arena)
{
	enum e_ast_type max_type;
	int		ltoken;
	size_t	new_range[2];
	t_ast_node	*node;

	node = NULL;
	max_type = get_highest_ast_type(xs, range);
	if (max_type == AST_LOGICAL)
	{
		ltoken = find_next_logical_token(xs, range);
		node = arena_calloc(arena, 1, sizeof(t_ast_node));
		  if (!node)
			return (NULL);
		node->token = xs[ltoken];
		node->type = AST_LOGICAL;
		new_range[0] = range[0];
		new_range[1] = (size_t)ltoken;
		range[0] = (size_t)ltoken + 1;
		if (add_node_to_parent(node, create_ast(xs, node, new_range, arena), arena) < 0)
			return (syntax_error("parsing logical node"));
		if (add_node_to_parent(node, create_ast(xs, node, range, arena), arena) < 0)
			return (syntax_error("parsing logical node"));
	}
	else if (max_type == AST_PAREN)
	{
		node = arena_calloc(arena, 1, sizeof(t_ast_node));
		if (!node)
			return (NULL);
		node->token = xs[range[0]];
		node->type = AST_PAREN;
		range[0]++; //Remove parentheses from range
		range[1]--;
		//node = create_ast(xs, parent, range, arena);
		if (add_node_to_parent(node, create_ast(xs, node, range, arena), arena) < 0)
			return (syntax_error("parsing parenthesis node"));
	}
	else if (max_type == AST_PIPELINE)
	{
		ltoken = find_next_token_of_type(xs, TOK_PIPE, range);
		node = arena_calloc(arena, 1, sizeof(t_ast_node));
		if (!node)
			return (NULL);
		node->token = xs[ltoken];
		node->type = AST_PIPELINE;
		new_range[0] = range[0];
		new_range[1] = (size_t)ltoken;
		range[0] = (size_t)ltoken + 1;
		if (add_node_to_parent(node, create_ast(xs, node, new_range, arena), arena) < 0)
			return (syntax_error("while parsping pipeline"));
		if (add_node_to_parent(node, create_ast(xs, node, range, arena), arena) < 0)
			return (syntax_error("while parsing pipeline"));
	}
	else if (max_type == AST_COMMAND)
	{
		
		node = create_command_node(xs, parent, range, arena);
		if (!node)
			return (syntax_error("while parsing command"));
	}
	else
		ft_printf("ERROR\n");
	return (node);
}
