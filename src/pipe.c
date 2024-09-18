/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 10:38:42 by pleander          #+#    #+#             */
/*   Updated: 2024/09/04 10:40:25 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**create_pipes(size_t n_pipes)
{
	size_t	i;
	int		**pipes;

	pipes = ft_calloc(n_pipes + 1, sizeof(int *));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < n_pipes)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			delete_pipes(pipes);
			return (NULL);
		}
		if (pipe(pipes[i]) == -1)
		{
			perror(NAME);
			delete_pipes(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	delete_pipes(int **pipes)
{
	size_t	i;

	i = 0;
	while (pipes[i])
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
		i++;
	}
	free(pipes);
}
