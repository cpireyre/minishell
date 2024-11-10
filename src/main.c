/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:08:02 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/10 14:11:35 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sysexits.h>
#include <errno.h>
#include "minishell.h"
#include "execute.h"

static int	minishell(t_arena arena, t_list *env);
static char	*arena_readline(t_arena arena, const char *prompt);
void		print_ast(t_ast_node *root, size_t level);

int	main(int argc, char **argv, char **envp)
{
	t_arena	arena;
	t_list	*env;
	int		exit_code;

	if (argc > 1)
		return (EX_USAGE + !ft_dprintf(2, "Usage: %s", argv[0]));
	env = init_env(envp);
	arena = arena_new();
	if (!env || !arena)
	{
		if (env)
			ft_lstclear(&env, &free);
		arena_dispose(&arena);
		ft_dprintf(2, "%s: Couldn't allocate memory", argv[0]);
		return (ENOMEM);
	}
	set_signal_handler();
	exit_code = minishell(arena, env);
	rl_clear_history();
	ft_lstclear(&env, &free);
	arena_dispose(&arena);
	return (exit_code);
}

static int	minishell(t_arena arena, t_list *env)
{
	char			*user_input_line;
	t_shell_status	status;
	t_ast_node		*ast;

	ft_bzero(&status, sizeof(status));
	set_status(&status);
	while (!status.should_exit)
	{
		user_input_line = arena_readline(arena, MINISHELL_PROMPT);
		status.should_exit = !user_input_line;
		if (!status.should_exit && *user_input_line)
		{
			add_history(user_input_line);
			ast = parse(arena, user_input_line, env, status.exit_code);
			if (ast)
				status = execute_ast(ast, env, arena, status.exit_code);
			else
				status.exit_code = 258;
		}
		arena_free(arena);
	}
	return (status.exit_code);
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
	else
	{
		ft_dprintf(2, "Error: couldn't allocate memory for user input\n");
		result = "";
	}
	free(line);
	return (result);
}

/**
 * @brief Initializes a new list of environmental variables
 *
 * @param envp Array of environmental variables, passed to main as third argument
 * @return pointer to list of environmental variables, otherwise NULL
 */
t_list	*init_env(char **envp)
{
	t_list	*head;
	t_list	*new;
	char	*envstr;

	if (!envp || !*envp)
		return (NULL);
	head = NULL;
	while (*envp)
	{
		envstr = ft_strdup(*envp++);
		if (!envstr)
		{
			ft_lstclear(&head, &free);
			return (NULL);
		}
		new = ft_lstnew(envstr);
		if (!new)
		{
			ft_lstclear(&head, &free);
			return (NULL);
		}
		ft_lstadd_back(&head, new);
	}
	return (head);
}
