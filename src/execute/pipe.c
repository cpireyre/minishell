/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 09:17:20 by pleander          #+#    #+#             */
/*   Updated: 2024/10/14 09:27:42 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"
#include "arena.h"

int	**make_pipes(int n_pipes, t_arena arena)
{
	int	**pipes;
	int	i;

	pipes = arena_alloc(arena, sizeof(int *));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < n_pipes)
	{
		pipes[i] = arena_alloc(arena, sizeof(int) * 2);
		if (!pipes)
			return (NULL);
		if (pipe(pipes[i]) < 0)
		{
			perror(NAME);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	close_pipes(int **pipes, int n_pipes)
{
	int	i;

	i = 0;
	while (i < n_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}
