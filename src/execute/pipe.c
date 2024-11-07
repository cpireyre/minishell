/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 09:17:20 by pleander          #+#    #+#             */
/*   Updated: 2024/11/06 11:13:29 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ast.h"
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

size_t	calculate_n_pipes(t_ast_node *ast)
{
	if (ast->type != AST_PIPELINE)
		return (0);
	return (calculate_n_pipes(ast->children[1]) + 1);
}

void	close_pipes(int **pipes, int n_pipes)
{
	int	i;

	i = 0;
	while (i < n_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}
