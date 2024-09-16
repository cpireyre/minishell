/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:55:41 by pleander          #+#    #+#             */
/*   Updated: 2024/09/16 11:57:29 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "minishell.h"

static int	open_file_fds(int file_fds[2], char *infile, char *outfile)
{
	file_fds[0] = open(infile, O_RDONLY);
	file_fds[1] = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_fds[0] < 0 || file_fds[1] < 0)
	{
		if (file_fds[0] < 0)
			ft_dprintf(STDERR_FILENO, "%s: %s: %s\n", NAME, infile,
				strerror(errno));
		if (file_fds[1] < 0)
			ft_dprintf(STDERR_FILENO, "%s: %s: %s\n", NAME, outfile,
				strerror(errno));
		if (file_fds[1] < 0)
			return (1);
		return (0);
	}
	return (1);
}

static int	open_heredoc_fds(t_fds *fds, char *outfile)
{
	fds->heredoc_pipe = malloc(sizeof(int) * 2);
	if (pipe(fds->heredoc_pipe) < 0)
		return (1);
	fds->file_fds[0] = dup(fds->heredoc_pipe[0]);
	close(fds->heredoc_pipe[0]);
	fds->file_fds[1] = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fds->file_fds[1] < 0)
		ft_dprintf(STDERR_FILENO, "%s: %s: %s\n", NAME, outfile,
			strerror(errno));
	if (fds->file_fds[1] < 0)
		return (1);
	return (0);
}

void	close_fds(t_fds *fds)
{
	if (fds->file_fds[0] != -1)
		close(fds->file_fds[0]);
	if (fds->file_fds[1] != -1)
		close(fds->file_fds[1]);
	if (fds->heredoc_pipe)
		free(fds->heredoc_pipe);
}

int	open_fds(t_fds *fds, t_pipeline* pipeline)
{
	if (pipeline->limiter)
		return (open_heredoc_fds(fds, pipeline->outfile));
	fds->heredoc_pipe = NULL;
	return (open_file_fds(fds->file_fds, pipeline->infile, pipeline->outfile));
}
