/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:51:52 by pleander          #+#    #+#             */
/*   Updated: 2024/10/10 14:18:31 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include "arena.h"
#include "libft.h"
#include "ast.h"
#include "execute.h"
#include "minishell.h"

static int	execute_cmd(t_ast_node *ast, t_list *env, t_arena arena, int pipefds[2]);
static int	wait_for_children(int *pid, size_t n_forks);

static	int	execute_single_command(t_ast_node *ast, t_list *env, t_arena arena)
{
	int		wstatus;
	int		e_status;
	pid_t	pid;
	int pipefds[2] = {-1, -1};

	pid = fork();
	if (pid == 0)
		execute_cmd(ast, env, arena, pipefds);
	if (pid == -1)
		return (1); //error exit?
	waitpid(pid, &wstatus, 0);
	if (WIFEXITED(wstatus))
		e_status = WEXITSTATUS(wstatus);
	else
		e_status = EXIT_FAILURE;
	return (e_status);
}

/**
 * @brief Executes the AST and returns the exit code
 *
 * @param ast AST node
 * @return exit code
 */
int	execute_ast(t_ast_node *ast, t_list	*env, t_arena arena)
{
	size_t	cur_child;
	pid_t	*child_pids;
	int pipefds[2] = {-1, -1};

	if (ast->type == AST_COMMAND)
		return (execute_single_command(ast, env, arena));

	child_pids = arena_alloc(arena, ast->n_children * sizeof(pid_t));
	if (child_pids)
		return (-1); // error exit
	cur_child = 0;
	while (cur_child < ast->n_children)
	{
		if (ast->type == AST_PIPELINE)
		{
			child_pids[cur_child] = fork();
			if (child_pids[cur_child] == 0)
				execute_cmd(ast, env, arena, pipefds);
			if (child_pids[cur_child] == -1)
				return (-1); // Error exit
		}
		else if (ast->type == AST_LOGICAL)
		{
			execute_ast(ast->children[cur_child], env, arena);
		}
		else
			ft_printf("We should not be here..\n");
		cur_child++;
	}
	wait_for_children(child_pids, ast->n_children);
	return (0);
}


static int	wait_for_children(int *pid, size_t n_forks)
{
	int		wstatus;
	int		e_status;
	size_t	i;

	wstatus = 0;
	e_status = 0;
	i = 0;
	while (i < n_forks)
	{
		wstatus = 0;
		if (pid[i] > 0)
		{
			waitpid(pid[i], &wstatus, 0);
			if (WIFEXITED(wstatus))
				e_status = WEXITSTATUS(wstatus);
		}
		else
			e_status = EXIT_FAILURE;
		i++;
	}
	return (e_status);
}

static int	execute_cmd(t_ast_node *ast, t_list *env, t_arena arena, int pipefds[2])
{
	t_command	cmd;
	
	cmd.infile = pipefds[0];
	cmd.outfile = pipefds[1];
	if (ast->type != AST_COMMAND)
	{
		ft_dprintf(2, "Error\n");
		return (1);
	}
	if (make_command(&cmd, ast, env, arena) < 0)
	{
		ft_dprintf(2, "Error making command\n");
		return (1);
	}
	if (cmd.infile > -1)
		dup2(cmd.infile, STDIN_FILENO);
	if (cmd.outfile > -1)
		dup2(cmd.outfile, STDOUT_FILENO);
	execve(cmd.path, (char *const *)cmd.args, make_raw_env_array(env, arena));
	perror(NAME);
	exit(1);
	return (1);
}

