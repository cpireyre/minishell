/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:22:05 by pleander          #+#    #+#             */
/*   Updated: 2024/11/28 09:44:13 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execute.h"
#include "signals.h"

static void	init_pipeline(t_command_context *con, t_list *env, t_ast_node *ast,
		t_arena arena)
{
	con->child_should_exit = 0;
	con->env = env;
	con->n_children = calculate_n_pipes(ast) + 1;
	con->pipes = make_pipes(con->n_children - 1, arena);
	con->cur_child = 0;
}

static pid_t	do_forking(t_command_context *con, int prev_exit, t_arena arena)
{
	pid_t		pid;
	t_command	cmd;
	int			err;

	cmd = (t_command){NULL, NULL, NULL, NULL, -1, -1};
	con->child_should_exit = 0;
	if (con->pipes && con->cur_child > 0)
		cmd.infile_fd = con->pipes[con->cur_child - 1][0];
	if (con->pipes && con->cur_child != con->n_children - 1)
		cmd.outfile_fd = con->pipes[con->cur_child][1];
	err = make_command(&cmd, con->ast, con->env, arena);
	if (err < 0)
	{
		con->child_should_exit = 1;
		if (err == INTERRUPTED_HEREDOC)
			con->should_quit_pipeline = true;
	}
	pid = fork();
	if (pid == 0)
		execute_cmd(&cmd, con, arena, prev_exit);
	return (pid);
}

static void	cleanup_pipeline(t_command_context *con, pid_t *child_pids,
			t_shell_status *status)
{
	close_pipes(con->pipes, con->n_children - 1);
	status->exit_code = wait_for_children(child_pids, con->n_children);
	if (con->should_quit_pipeline)
		status->exit_code = 128 + SIGINT;
	status->should_exit = false;
	status->sigint_received = false;
}

static void	set_next_child(t_ast_node *ast, t_command_context *con)
{
	if (ast->type == AST_PIPELINE)
		con->ast = ast->children[0];
	else if (ast->type == AST_COMMAND)
		con->ast = ast;
}

t_shell_status	execute_pipeline(
		t_ast_node *ast, t_list *env, t_arena arena, int prev_exit)
{
	t_command_context	con;
	pid_t				*child_pids;
	t_shell_status		status;

	con.should_quit_pipeline = false;
	set_signal_handlers(SIG_IGN, SIG_IGN);
	init_pipeline(&con, env, ast, arena);
	child_pids = arena_calloc(arena, con.n_children, sizeof(pid_t));
	if (!child_pids || !con.pipes)
		return ((t_shell_status){.exit_code = -1});
	while (ast->type == AST_PIPELINE || ast->type == AST_COMMAND)
	{
		set_next_child(ast, &con);
		child_pids[con.cur_child] = do_forking(&con, prev_exit, arena);
		if (con.should_quit_pipeline)
			break ;
		if (child_pids[con.cur_child] == -1)
			return ((t_shell_status){.exit_code = -1, .should_exit = true});
		if (ast->type == AST_COMMAND)
			break ;
		ast = ast->children[1];
		con.cur_child++;
	}
	cleanup_pipeline(&con, child_pids, &status);
	return (status);
}
