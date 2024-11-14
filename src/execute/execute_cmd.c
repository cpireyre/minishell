/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 08:31:14 by pleander          #+#    #+#             */
/*   Updated: 2024/11/14 09:27:08 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "minishell.h"

int	execute_cmd(t_command_context *con, t_arena arena, int prev_exit)
{
	t_command	cmd;	

	cmd.infile_fd = -1;
	cmd.outfile_fd = -1;
	if (con->pipes && con->cur_child > 0)
		cmd.infile_fd = con->pipes[con->cur_child - 1][0];
	if (con->pipes && con->cur_child != con->n_children - 1)
		cmd.outfile_fd = con->pipes[con->cur_child][1];
	if (make_command(&cmd, con->ast, con->env, arena) < 0)
		exit (1);
	if (cmd.infile_fd > -1)
		dup2(cmd.infile_fd, STDIN_FILENO);
	if (cmd.outfile_fd > -1)
		dup2(cmd.outfile_fd, STDOUT_FILENO);
	if (con->pipes)
		close_pipes(con->pipes, con->n_children - 1);
	if (is_builtin(cmd.path))
		exit(run_builtin(cmd.path, cmd.args, &con->env, prev_exit).exit_code);
	if (!is_path(cmd.path))
	{
		dprintf(STDERR_FILENO, "%s: %s\n", cmd.path, "command not found");
		exit(EX_NOTFOUND);
	}
	exit (minishell_execve(cmd.path, (char **)cmd.args,
			make_raw_env_array(con->env, arena)));
}
