/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:46:51 by pleander          #+#    #+#             */
/*   Updated: 2024/09/16 11:58:15 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	spawn_child(t_fds *fds, t_context *con, t_children *children, t_cmd *cmds)
{
	if (children->n_children == 0)
	{
		if (fds->file_fds[0] < 0 || dup2(fds->file_fds[0], STDIN_FILENO) < 0)
			return (child_error_return(fds->pipes, con, children));
	}
	else
		if (dup2(fds->pipes[children->n_children - 1][0], STDIN_FILENO) < 0)
			return (child_error_return(fds->pipes, con, children));
	if (children->n_children == con->n_cmds - 1)
	{
		if (fds->file_fds[1] < 0 || dup2(fds->file_fds[1], STDOUT_FILENO) < 0)
			return (child_error_return(fds->pipes, con, children));
	}
	else
		if (dup2(fds->pipes[children->n_children][1], STDOUT_FILENO) < 0)
			return (child_error_return(fds->pipes, con, children));
	delete_pipes(fds->pipes, con->n_cmds - 1);
	close_fds(fds);
	if (execve(cmds[children->n_children].exec_path,
			cmds[children->n_children].args, con->envp) == -1)
		return (execve_error_handler(cmds, children, con));
	exit(EXIT_FAILURE);
}
