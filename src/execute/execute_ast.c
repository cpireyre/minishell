/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:51:52 by pleander          #+#    #+#             */
/*   Updated: 2024/11/22 10:55:35 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include "arena.h"
#include "libft.h"
#include "ast.h"
#include "execute.h"

static bool	print_signal_newline(int wstatus);

/**
 * @brief Executes the AST and returns the exit code
 *
 * @param ast AST node
 * @return shell status (exit code + should_exit flag)
 */
t_shell_status	execute_ast(t_ast_node *ast, t_list	*env, t_arena arena,
		int prev_exit)
{
	t_shell_status	status;

	status.exit_code = 0;
	status.should_exit = false;
	status.sigint_received = false;
	if (ast->type == AST_COMMAND && ast->n_children > 0)
		status = execute_single_command(ast, env, arena, prev_exit);
	else if (ast->type == AST_PIPELINE)
		status = execute_pipeline(ast, env, arena, prev_exit);
	else if (ast->type == AST_LOGICAL)
		status = execute_logicals(ast, env, arena, prev_exit);
	else if (ast->type == AST_PAREN)
		status = execute_ast(ast->children[0], env, arena, prev_exit);
	return (status);
}

/**
 * @brief Wait for children to finish running and get return their
 * exit statuses
 *
 * @param pid: array of pids to wait for
 * @param n_forks: how many forks i.e. pids there are
 * @return: the final exit status
 */
int	wait_for_children(int *pid, size_t n_forks)
{
	int		wstatus;
	int		e_status;
	size_t	i;
	bool	printed_newline;

	wstatus = 0;
	e_status = 0;
	i = 0;
	printed_newline = false;
	while (i < n_forks)
	{
		wstatus = 0;
		if (pid[i] > 0)
		{
			waitpid(pid[i], &wstatus, 0);
			e_status = get_exit_code(wstatus);
			if (!printed_newline)
				printed_newline = print_signal_newline(wstatus);
		}
		else
			e_status = EXIT_FAILURE;
		i++;
	}
	return (e_status);
}

int	get_exit_code(int wstatus)
{
	int	ret;

	ret = 0;
	if (WIFEXITED(wstatus))
		ret = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
		ret = 128 + WTERMSIG(wstatus);
	return (ret);
}

static bool	print_signal_newline(int wstatus)
{
	return (WIFSIGNALED(wstatus)
		&& (WTERMSIG(wstatus) == SIGQUIT || WTERMSIG(wstatus) == SIGINT)
		&& write(1, "\n", 1));
}
