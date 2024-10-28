/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:51:52 by pleander          #+#    #+#             */
/*   Updated: 2024/10/28 11:49:38 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include "arena.h"
#include "libft.h"
#include "ast.h"
#include "execute.h"
#include "minishell.h"
#include "tokenize.h"

static int	execute_cmd(t_command_context *con, t_arena arena);
static int	wait_for_children(int *pid, size_t n_forks);
static	int	execute_single_command(t_ast_node *ast, t_list *env, t_arena arena);
static	int	execute_pipeline(t_ast_node *ast, t_list *env, t_arena arena);
static int	execute_logicals(t_ast_node *ast, t_list *env, t_arena arena);

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
		status = execute_logicals(ast, env, arena);
	else
	{
		status = 1;
		ft_printf("We should not be here..\n");
	}
	return (status);
}

static int	execute_logicals(t_ast_node *ast, t_list *env, t_arena arena)
{
	t_ast_node	*cur;
	int	s;
	
	cur = ast;
	s = execute_ast(cur->children[0], env, arena);
	while (cur->type == AST_LOGICAL)
	{
		if (cur->token.type == TOK_LOGICAL_AND)
		{
			if (cur->children[1]->type == AST_LOGICAL)
				s = !((s == 0) && (execute_ast(cur->children[1]->children[0], env, arena) == 0));
			else
				s = !((s == 0) && (execute_ast(cur->children[1], env, arena) == 0));
		}
		if (cur->token.type == TOK_LOGICAL_OR)
		{
			if (cur->children[1]->type == AST_LOGICAL)
				s = !((s == 0) || (execute_ast(cur->children[1]->children[0], env, arena) == 0));
			else
				s = !((s == 0) || (execute_ast(cur->children[1], env, arena) == 0));
		}
		if (!s)
			break ;
		cur = cur->children[1];
	}
	return (s);
}

static int	execute_pipeline(t_ast_node *ast, t_list *env, t_arena arena)
{
	t_command_context	con;
	pid_t	*child_pids;
	int		status;

	con.env = env;
	con.n_children = calculate_n_pipes(ast) + 1;
	con.pipes = make_pipes(con.n_children - 1, arena);
	child_pids = arena_alloc(arena, (con.n_children) * sizeof(pid_t));
	if (!child_pids || !con.pipes)
		return (-1);
	con.cur_child = 0;
	while (ast->type == AST_PIPELINE || ast->type == AST_COMMAND)
	{
		if (ast->type == AST_PIPELINE)
			con.ast = ast->children[0]; //left child i.e command
		else if (ast->type == AST_COMMAND)
			con.ast = ast;
		child_pids[con.cur_child] = fork();
		if (child_pids[con.cur_child] == 0)
			execute_cmd(&con, arena);
		if (child_pids[con.cur_child] == -1)
			return (-1); // Error exit
		if (ast->type == AST_COMMAND)
			break ;
		ast = ast->children[1]; // next pipe opreator
		con.cur_child++;
	}
	close_pipes(con.pipes, con.n_children - 1);
	status = wait_for_children(child_pids, con.n_children);
	return (status);
}

static	int	execute_single_command(t_ast_node *ast, t_list *env, t_arena arena)
{
	t_command_context	con;
	int					status;
	pid_t				*child_pids;

	con.ast = ast;
	con.env = env;
	con.pipes = NULL;
	con.cur_child = 0;
	con.n_children = 1;
	child_pids = arena_alloc(arena, (con.n_children) * sizeof(pid_t));
	if (!child_pids)
		return (-1);
	child_pids[0] = fork();
	if (child_pids[0] == 0)
		execute_cmd(&con, arena);
	else if (child_pids[0] < 0)
		return (-1);
	status = wait_for_children(child_pids, con.n_children);
	return (status);
}

static void	print_command(t_command *cmd)
{
	int	i;

	ft_dprintf(2, "[%s]:", cmd->path);
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		ft_dprintf(2, " (%d: %s)", i, cmd->args[i]);
		i++;
	}
	if (cmd->infile)
		ft_dprintf(2, " (< %s)", cmd->infile);
	if (cmd->outfile)
		ft_dprintf(2, " (> %s)", cmd->outfile);
	ft_dprintf(2, "\n");
}

static int	execute_cmd(t_command_context *con, t_arena arena)
{
	t_command	cmd;	
	
	if (make_command(&cmd, con->ast, con->env, arena) < 0)
	{
		ft_dprintf(2, "Error making command\n");
		return (1);
	}
	if (con->pipes && con->cur_child > 0)
		cmd.infile_fd = con->pipes[con->cur_child - 1][0];
	if (con->pipes && con->cur_child != con->n_children - 1)
		cmd.outfile_fd = con->pipes[con->cur_child][1];
	if (cmd.infile_fd > -1)
		dup2(cmd.infile_fd, STDIN_FILENO);
	if (cmd.outfile_fd > -1)
		dup2(cmd.outfile_fd, STDOUT_FILENO);
	if (con->pipes)
		close_pipes(con->pipes, con->n_children - 1);
	if (DEBUG)
		print_command(&cmd);
	if (is_builtin(cmd.path))
		run_builtin(cmd.path, cmd.args, &con->env);
	execve(cmd.path, (char **)cmd.args, make_raw_env_array(con->env, arena));
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
