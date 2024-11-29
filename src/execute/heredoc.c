/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:35:35 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/29 10:14:27 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "minishell.h"
#include "signals.h"
#include "libft.h"

int	g_signal_received;

static enum e_heredoc_state	take(int fd, const char *delimiter);
static void					wake(int sig);
static int					quit(void);

int	handle_redir_heredoc(t_command *cmd, t_ast_node *ast)
{
	int						hdoc_pipe[2];
	enum e_heredoc_state	state;
	const char				*delimiter = ast->children[0]->token.value;

	if (pipe(hdoc_pipe) < 0)
	{
		ft_dprintf(2, "%s: %s: %s", NAME, cmd->infile, strerror(errno));
		return (-1);
	}
	cmd->infile_fd = hdoc_pipe[PIPE_READ];
	rl_event_hook = quit;
	set_signal_handlers(SIG_IGN, wake);
	state = HEREDOC_CONTINUE;
	while (state == HEREDOC_CONTINUE)
		state = take(hdoc_pipe[PIPE_WRITE], delimiter);
	close(hdoc_pipe[PIPE_WRITE]);
	if (state == HEREDOC_SIGINT)
		return (INTERRUPTED_HEREDOC);
	return (0);
}

static enum e_heredoc_state	take(int fd, const char *delimiter)
{
	char	*line;
	bool	found_delimiter;
	int		error;

	if (isatty(STDIN_FILENO))
		line = readline("> ");
	else
		line = get_next_line(STDIN_FILENO);
	if (!line)
		return (HEREDOC_QUIT);
	found_delimiter = ft_streq(delimiter, line);
	if (!found_delimiter)
		error = ft_dprintf(fd, "%s\n", line);
	free(line);
	if (g_signal_received)
	{
		g_signal_received = 0;
		return (HEREDOC_SIGINT);
	}
	else if (error == -1 || found_delimiter)
		return (HEREDOC_QUIT);
	return (HEREDOC_CONTINUE);
}

static void	wake(int sig)
{
	g_signal_received = sig;
}

static int	quit(void)
{
	if (g_signal_received)
		rl_done = 1;
	return (0);
}
