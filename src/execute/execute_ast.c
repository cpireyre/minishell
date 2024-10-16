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

static int	execute_cmd(t_command_context *con, t_arena arena);
static int	wait_for_children(int *pid, size_t n_forks);
static	int	execute_single_command(t_ast_node *ast, t_list *env, t_arena arena);
static	int	execute_pipeline(t_ast_node *ast, t_list *env, t_arena arena);

/**
 * @brief Executes the AST and returns the exit code
 *
 * @param ast AST node
 * @return exit code
 */
int	execute_ast(t_ast_node *ast, t_list	*env, t_arena arena)
{
	int	status;

	if (ast->type == AST_COMMAND)
		status = execute_single_command(ast, env, arena);
	else if (ast->type == AST_PIPELINE)
		status = execute_pipeline(ast, env, arena);
	else if (ast->type == AST_LOGICAL)
	{
		//fork()
		//execute_ast(ast->children[cur_child], env, arena, NULL);
		status = 0;
	}
	else
	{
		status = 1;
		ft_printf("We should not be here..\n");
	}
	return (status);
}

static	int	execute_pipeline(t_ast_node *ast, t_list *env, t_arena arena)
{
	t_command_context	con;
	pid_t	*child_pids;
	int		status;

	con.ast = ast;
	con.env = env;
	con.pipes = make_pipes(ast->n_children - 1, arena);
	child_pids = arena_alloc(arena, ast->n_children * sizeof(pid_t));
	if (!child_pids || !con.pipes)
		return (-1);
	con.cur_child = 0;
	while (con.cur_child < ast->n_children)
	{
		child_pids[con.cur_child] = fork();
		if (child_pids[con.cur_child] == 0)
			execute_cmd(&con, arena);
		if (child_pids[con.cur_child] == -1)
			return (-1); // Error exit
		con.cur_child++;
	}
	close_pipes(con.pipes, ast->n_children - 1);
	status = wait_for_children(child_pids, ast->n_children);
	return (status);
}

static	int	execute_single_command(t_ast_node *ast, t_list *env, t_arena arena)
{
	int		wstatus;
	int		e_status;
	pid_t	pid;


	// pid = fork();
	// if (pid == 0)
	// 	execute_cmd(ast, env, arena, NULL);
	// if (pid == -1)
	// 	return (1); //error exit?
	// waitpid(pid, &wstatus, 0);
	// if (WIFEXITED(wstatus))
	// 	e_status = WEXITSTATUS(wstatus);
	// else
	// 	e_status = EXIT_FAILURE;
	// return (e_status);
	return (0);
}

//static int	execute_cmd(t_ast_node *ast, t_list *env, t_arena arena, int **pipes)
static int	execute_cmd(t_command_context *con, t_arena arena)
{
	t_command	cmd;	
	int			pipefds[2];
	
	if (make_command(&cmd, con->ast, con->env, arena) < 0)
	{
		ft_dprintf(2, "Error making command\n");
		return (1);
	}
	if (con->cur_child != 0)
		pipefds[0] = con->pipes[con->cur_child - 1][0];
	if (con->cur_child != con->ast->n_children - 1)
		pipefds[1] = con->pipes[con->cur_child][1];
	if (cmd.infile > -1)
		dup2(cmd.infile, STDIN_FILENO);
	if (cmd.outfile > -1)
		dup2(cmd.outfile, STDOUT_FILENO);
	execve(cmd.path, (char *const *)cmd.args, make_raw_env_array(con->env, arena));
	perror(NAME);
	exit(1);
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
