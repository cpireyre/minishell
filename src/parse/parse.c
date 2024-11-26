/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:57:35 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/26 10:07:31 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ast.h"
#include "glob.h"
#include "expand.h"
#include "minishell.h"

static bool	parse_validate_string(const char *str);
static char	*get_ast_type(enum e_ast_type t);
void		split_words(t_arena arena, t_ast_node *ast);
void		remove_quotes(t_arena arena, t_ast_node *ast);

t_ast_node	*parse(t_arena arena, char *user_input_line,
			t_list *env, int exit_code)
{
	t_token		*xs;
	t_ast_node	*ast;
	size_t		range[2];
	const bool	parse_ok = parse_validate_string(user_input_line);

	if (!parse_ok)
	{
		ft_dprintf(2, NAME ": syntax error\n");
		return (NULL);
	}
	ast = NULL;
	xs = tokenize(arena, user_input_line);
	if (xs && xs[0].type != TOK_END)
	{
		ft_bzero(range, sizeof(range));
		range[1] = count_toks(xs) - 1;
		ast = create_ast(xs, range, arena);
		if (!expand(ast, arena, env, exit_code))
			return (NULL);
		glob(arena, ast);
		split_words(arena, ast);
		remove_quotes(arena, ast);
	}
	return (ast);
}

void	print_ast(t_ast_node *root, size_t level)
{
	size_t	i;
	size_t	pad;

	if (!root)
		return ;
	pad = 0;
	while (pad++ < level * 2)
		ft_printf(" ");
	ft_printf("%s [%s (%s)]\n", get_ast_type(root->type),
		ast_show_type(root->token.type), root->token.value);
	level++;
	i = 0;
	while (i < root->n_children)
		print_ast(root->children[i++], level);
}

static char	*get_ast_type(enum e_ast_type t)
{
	if (t == AST_PAREN)
		return ("AST_PAREN");
	else if (t == AST_LOGICAL)
		return ("AST_LOGICAL");
	else if (t == AST_PIPELINE)
		return ("AST_PIPELINE");
	else if (t == AST_COMMAND)
		return ("AST_COMMAND");
	else if (t == AST_WORD)
		return ("AST_WORD");
	else if (t == AST_REDIR)
		return ("AST_REDIR");
	return (NULL);
}

static bool	parse_validate_string(const char *str)
{
	int		paren_count;
	bool	in_single_quotes;
	bool	in_double_quotes;

	paren_count = 0;
	in_single_quotes = false;
	in_double_quotes = false;
	while (*str)
	{
		if (!in_double_quotes && *str == '\'')
			in_single_quotes = !in_single_quotes;
		if (!in_single_quotes && *str == '"')
			in_double_quotes = !in_double_quotes;
		if (!in_double_quotes && !in_single_quotes)
			paren_count += (*str == '(') - (*str == ')');
		if (paren_count < 0)
			return (false);
		str++;
	}
	return (!paren_count && !in_single_quotes && !in_double_quotes);
}
