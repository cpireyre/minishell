/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:41:44 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/07 10:22:24 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <readline/readline.h>
#include "minishell.h"

void	catch_signal(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		set_status(NULL);
	}
}

void	set_status(t_shell_status *status)
{
	static t_shell_status	*status_pointer;

	if (!status)
		status_pointer->exit_code = SIGINT + 128;
	else
		status_pointer = status;
}

int	set_signal_handler(void)
{
	struct sigaction	sa;

	sa = (struct sigaction){0};
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, 0);
	sa.sa_handler = &catch_signal;
	sigaction(SIGINT, &sa, 0);
	return (0);
}
