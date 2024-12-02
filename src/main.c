/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:08:02 by copireyr          #+#    #+#             */
/*   Updated: 2024/12/02 13:58:42 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sysexits.h>
#include <errno.h>
#include "minishell.h"
#include "signals.h"
#include "execute.h"

static int	minishell(t_arena arena, t_list *env);
static char	*arena_readline(t_arena arena, const char *prompt);
void		print_ast(t_ast_node *root, size_t level);
static int	add_to_envlist(t_list **head, const char *str);

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
	set_signal_handlers(SIG_IGN, redisplay_prompt);
	exit_code = minishell(arena, env);
	rl_clear_history();
	ft_lstclear(&env, &free);
	arena_dispose(&arena);
	if (isatty(0))
		ft_dprintf(2, "exit\n");
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
		set_signal_handlers(SIG_IGN, redisplay_prompt);
		user_input_line = arena_readline(arena, MINISHELL_PROMPT);
		status.should_exit = !user_input_line;
		if (!status.should_exit && *user_input_line)
		{
			add_history(user_input_line);
			ast = parse(arena, user_input_line, env, status.exit_code);
			if (ast)
				status = execute_ast(ast, env, arena, status.exit_code);
			else
				status.exit_code = 2;
		}
		arena_free(arena);
		ft_printf(ANSI_RESET);
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
	char	*pwd;
	char	*pwd_var;

	head = NULL;
	if (!envp || !*envp)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
			return (NULL);
		pwd_var = ft_strjoin("PWD=", pwd);
		free(pwd);
		if (!pwd_var)
			return (NULL);
		if (!add_to_envlist(&head, pwd_var))
		{
			free(pwd_var);
			return (NULL);
		}
		free(pwd_var);
	}
	while (*envp)
		if (!add_to_envlist(&head, *envp++))
			ft_lstclear(&head, &free);
	return (head);
}

static int	add_to_envlist(t_list **head, const char *str)
{
	char	*envstr;
	t_list	*new;

	envstr = ft_strdup(str);
	if (!envstr)
		return (0);
	new = ft_lstnew(envstr);
	if (!new)
	{
		free(envstr);
		return (0);
	}
	ft_lstadd_back(head, new);
	return (1);
}
