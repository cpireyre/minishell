/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 14:50:55 by pleander          #+#    #+#             */
/*   Updated: 2024/09/16 11:54:30 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include "minishell.h"

static int	run_pipeline(t_pipeline *pipeline, int	**pipes);
static int	wait_for_children(int *pid, size_t n_forks);

/**
 * @brief Executes a pipeline. Does not expand variables.
 *
 * @param pipeline The pipeline
 * @return The exit code of the pipeline
 */
int	pipeline(t_pipeline *pipeline)
{
	int retval;
	int	**pipes;

	pipes = create_pipes(pipeline->n_cmds - 1);
	if (!pipes)
	{
		//clear and clean
		return (1);
	}
	retval = run_pipeline(pipeline, pipes);
	return (retval);
}

static int	run_pipeline(t_pipeline *pipeline, int	**pipes)
{
	printf("Running pipeline..\n");
	t_children	children;
	int			e_status;

	children.child_pids = malloc(sizeof(pid_t) * pipeline->n_cmds);
	if (!children.child_pids)
		return (1);
	children.n_children = 0;
	while (children.n_children < pipeline->n_cmds)
	{
		children.child_pids[children.n_children] = fork();
		if (children.child_pids[children.n_children] == 0)
			return (spawn_child(&pipeline->cmds[children.n_children], pipes));
		if (children.child_pids[children.n_children] == -1)
			continue ;
		children.n_children++;
	}
	//delete_pipes(fds->pipes, con->n_cmds - 1);
	e_status = wait_for_children(children.child_pids, children.n_children);
	//free(children.child_pids);
	return (e_status);
}

static int	wait_for_children(int *pid, size_t n_forks)
{
	int		wstatus;
	int		e_status;
	size_t	i;

	wstatus = 0;
	e_status = 0;
	i = 0;
	while (i < n_forks)
	{
		wstatus = 0;
		if (pid[i] > 0)
		{
			waitpid(pid[i], &wstatus, 0);
			if (WIFEXITED(wstatus))
				e_status = WEXITSTATUS(wstatus);
		}
		else
			e_status = EXIT_FAILURE;
		i++;
	}
	return (e_status);
}
