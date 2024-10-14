/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:57:35 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/14 14:30:07 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ast.h"
#include "glob.h"
#include "expand.h"

static char	*get_ast_type(enum e_ast_type t);
void		split_words(t_arena arena, t_ast_node *ast);

t_ast_node	*parse(t_arena arena, char *user_input_line, t_list *env)
{
	t_token		*xs;
	t_ast_node	*ast;
	size_t		range[2];

	ast = NULL;
	xs = tokenize(arena, user_input_line);
	if (xs)
	{
		ft_bzero(range, sizeof(range));
		range[1] = count_toks(xs) - 1;
		ast = create_ast(xs, ast, range, arena);
		expand(ast, arena, env);
		glob(arena, ast);
		split_words(arena, ast);
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
	ft_printf("%s ", get_ast_type(root->type));
	ft_printf("[%s (", ast_show_type(root->token.type));
	i = 0;
	ft_printf("%s", root->token.value);
	ft_printf(")]\n");
	level++;
	i = 0;
	while (i < root->n_children)
		print_ast(root->children[i++], level);
}

static char	*get_ast_type(enum e_ast_type t)
{
	if (t == AST_LOGICAL)
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
