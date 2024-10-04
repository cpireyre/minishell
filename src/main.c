/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:08:02 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/04 14:13:49 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sysexits.h>
#include <errno.h>
#include <stdbool.h>
#include "minishell.h"
#include "tokenize.h"
#include "ast.h"
#include "libft.h"

static char	*arena_readline(t_arena arena, const char *prompt);
void		print_ast(t_ast_node *root, size_t level);

int	main(int argc, char **argv, char **envp)
{
	t_list		**env;
	char		*user_input_line;
	bool		should_exit_shell;
	t_ast_node	*ast;
	t_arena		arena;

	if (argc > 1)
	{
		ft_dprintf(2, "Usage: %s", argv[0]);
		return (EX_USAGE);
	}
	set_signal_handler();
	env = init_env(envp);
	if (!env)
		return (ENOMEM);
	should_exit_shell = false;
	while (!should_exit_shell)
	{
		arena = arena_new();
		if (!arena)
			break ;
		user_input_line = arena_readline(arena, MINISHELL_PROMPT);
		should_exit_shell = !user_input_line;
		if (!should_exit_shell && *user_input_line)
		{
			add_history(user_input_line);
			ast = parse(arena, user_input_line, *env);
			print_ast(ast, 0);
		}
		arena_dispose(&arena);
	}
	ft_lstclear(env, &free);
	free(env);
	return (0);
}

static char	*arena_readline(t_arena arena, const char *prompt)
{
	char	*line;
	size_t	length_line;
	char	*result;

	line = readline(prompt);
	if (!line)
		return (NULL);
	length_line = ft_strlen(line);
	result = arena_calloc(arena, 1, length_line + 1);
	if (result)
		ft_memcpy(result, line, length_line);
	free(line);
	return (result);
}
