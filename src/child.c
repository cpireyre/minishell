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

#include <fcntl.h>
#include <unistd.h>
#include "minishell.h"
#include "libft.h"

int	spawn_child(t_command *cmd, int **pipes)
{
	int	fd;

	if (cmd->infile != NULL) //if infile is not null we should read from the previous pipe
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0 || dup2(fd, STDIN_FILENO) < 0)
		{
			ft_dprintf(2, "Failed to open infile at %s\n", cmd->infile);
			return (-1);
		}
		close(fd);
	}
	else
		if (dup2(pipes[cmd->i - 1][0], STDIN_FILENO) < 0)
			return (-1);
	if (cmd->outfile != NULL)
	{
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0 || dup2(fd, STDOUT_FILENO) < 0)
		{
			ft_dprintf(2, "Failed to open outfile at %s\n", cmd->outfile);
			return (-1);
		}
		close(fd);
	}
	else
		if (dup2(pipes[cmd->i][1], STDOUT_FILENO) < 0)
			return (-1);
			//return (child_error_return(fds->pipes, con, children));

	//delete_pipes(fds->pipes, con->n_cmds - 1);
	if (execve(cmd->path, cmd->args, NULL) == -1)
		return (-1);
		//return (execve_error_handler(cmds, children, con));
	exit(EXIT_FAILURE);
}
