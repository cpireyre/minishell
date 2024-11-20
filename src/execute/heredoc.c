/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:35:35 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/20 14:19:12 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <sys/wait.h>
#include "minishell.h"
#include "signals.h"
#include "libft.h"

static void	pipe_heredoc_input(t_command *cmd, int hdoc_pipe[2])
{
	char	*line;

	set_signal_handlers(SIG_IGN, SIG_DFL);
	close_fd_if_open(&cmd->infile_fd);
	line = readline("> ");
	if (!line)
		exit(1);
	while (!ft_streq(line, cmd->infile))
	{
		ft_dprintf(hdoc_pipe[1], "%s\n", line);
		free(line);
		line = readline("> ");
		if (!line)
			exit(1);
	}
	free(line);
	close(hdoc_pipe[0]);
	close(hdoc_pipe[1]);
}

int	handle_redir_heredoc(t_command *cmd, t_ast_node *ast)
{
	int		hdoc_pipe[2];
	pid_t	child_pid;
	int		wstatus;

	cmd->infile = (char *)ast->children[0]->token.value;
	if (pipe(hdoc_pipe) < 0)
		return (-1 + !ft_dprintf(
				2, "%s: %s: %s", NAME, cmd->infile, strerror(errno)));
	cmd->infile_fd = hdoc_pipe[0];
	set_signal_handlers(SIG_IGN, SIG_IGN);
	child_pid = fork();
	if (child_pid == 0)
	{
		pipe_heredoc_input(cmd, hdoc_pipe);
		exit(0);
	}
	close(hdoc_pipe[1]);
	waitpid(child_pid, &wstatus, 0);
	if (WIFSIGNALED(wstatus) && WTERMSIG(wstatus) == SIGINT)
	{
		write(1, "\n", 1);
		return (INTERRUPTED_HEREDOC);
	}
	return (0);
}
