/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:08:02 by copireyr          #+#    #+#             */
/*   Updated: 2024/08/28 12:19:34 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

int	main(void)
{
	char	*user_input_line;

	while (1)
	{
		user_input_line = readline("λ> ");
		free(user_input_line);
	}
}
