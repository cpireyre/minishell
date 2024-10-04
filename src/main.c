/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:08:02 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/04 13:46:51 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include "minishell.h"
#include "tokenize.h"
#include "ast.h"
#include "libft.h"

char	*arena_readline(t_arena arena, const char *prompt)
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

int	main(int argc, char **argv, char **envp)
{
	t_list	**env;
	t_token	*xs;
	char	*user_input_line;
	bool	should_exit_shell;
	t_arena arena;

	if (argc > 1)
	{
		ft_dprintf(2, "%s: error: program does not accept any arguments\n", argv[0]);
		return (64);
	}
	set_signal_handler();
	env = init_env(envp);
	if (!env)
		exit(1);
	should_exit_shell = false;
	while (!should_exit_shell)
	{
		arena = arena_new();
		if (!arena)
			break ;
		user_input_line = arena_readline(arena, MINISHELL_PROMPT);
		should_exit_shell = !user_input_line || ft_streq(user_input_line, "exit");
		if (!should_exit_shell && *user_input_line)
		{
			add_history(user_input_line);
			t_ast_node *ast = parse(arena, user_input_line, *env);
			print_ast(ast, 0);
		}
		arena_dispose(&arena);
	}
	ft_lstclear(env, &free);
	free(env);
	return (0);
}
