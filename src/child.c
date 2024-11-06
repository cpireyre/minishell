/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:46:51 by pleander          #+#    #+#             */
/*   Updated: 2024/11/06 11:09:34 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "minishell.h"

static void	err_exit(void)
{
	perror(NAME);
	exit(EXIT_FAILURE);
}

/**
 * @brief Spawns a child process for the given command. Note that execve()
 * will clean all the heap memory of the process that is being replaced.
 *
 * @param cmd 
 * @param pipes 
 * @param env 
 */
void	spawn_child(t_command *cmd, int **pipes, char **env)
{
	int	fd;

	if (cmd->infile != NULL)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0 || dup2(fd, STDIN_FILENO) < 0)
			err_exit();
		close(fd);
	}
	else
	{
		if (cmd->hdoc_delim)
		{
			ft_dprintf(2, "TODO: Implement heredoc\n");
			exit(1);
		}
		if (dup2(pipes[cmd->i - 1][0], STDIN_FILENO) < 0)
			err_exit();
	}
	if (cmd->outfile != NULL)
	{
		if (cmd->of_append)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT, 0644);
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0 || dup2(fd, STDOUT_FILENO) < 0)
			err_exit();
		close(fd);
	}
	else
		if (dup2(pipes[cmd->i][1], STDOUT_FILENO) < 0)
			err_exit();
	delete_pipes(pipes);
	execve(cmd->path, cmd->args, env);
	err_exit();
}
