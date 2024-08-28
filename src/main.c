/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:08:02 by copireyr          #+#    #+#             */
/*   Updated: 2024/08/28 15:21:03 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft.h"

int	main(void)
{
	char	*user_input_line;
	int		should_exit_shell;

	while (1)
	{
		user_input_line = readline("λ> ");
		should_exit_shell = ft_streq(user_input_line, "exit");
		free(user_input_line);
		if (should_exit_shell)
			break ;
	}
	return (0);
}
