/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:41:44 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/16 09:28:40 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>

static int	g_signal_received;

void	catch_signal(int sig)
{
	g_signal_received = sig;
}

int	sigint_refresh_prompt(void)
{
	if (g_signal_received == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	g_signal_received = 0;
	return (0);
}

int	set_signal_handler(void)
{
	struct sigaction	sa;

	sa = (struct sigaction){0};
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, 0);
	sa.sa_handler = &catch_signal;
	sigaction(SIGINT, &sa, 0);
	rl_event_hook = (rl_hook_func_t *)&sigint_refresh_prompt;
	return (0);
}
