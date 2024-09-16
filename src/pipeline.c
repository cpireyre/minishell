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

#include "minishell.h"

static int	run_pipeline(t_pipeline *pipeline, int	**pipes);

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
			//return (spawn_child(pipeline, pipes, &children));
		// if (children.child_pids[children.n_children] == -1)
		// {
		// 	children.n_children++;
		// 	continue ;
		// }
		children.n_children++;
	}
	delete_pipes(fds->pipes, con->n_cmds - 1);
	e_status = wait_for_children(children.child_pids, children.n_children);
	free(children.child_pids);
	return (e_status);
}
