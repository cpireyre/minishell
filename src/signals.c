/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:41:44 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/18 11:36:26 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <readline/readline.h>
#include "minishell.h"

void	redisplay_prompt(int sig)
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

void	nop(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	set_status(NULL);
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
	sa.sa_handler = &redisplay_prompt;
	sigaction(SIGINT, &sa, 0);
	return (0);
}

int	set_nop_handler(void)
{
	struct sigaction	sa;

	sa = (struct sigaction){0};
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, 0);
	sa.sa_handler = &nop;
	sigaction(SIGINT, &sa, 0);
	return (0);
}
