/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:08:02 by copireyr          #+#    #+#             */
/*   Updated: 2024/09/03 15:38:47 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft.h"

int	main(int argc, char **argv, char **envp)
{
	t_list	**env;
	char	*user_input_line;
	int		should_exit_shell;

	if (argc > 1)
	{
		ft_dprintf(2, "minishell: error: program does not accept any arguments\n");
		exit(1);
	}
	(void)argv;
	env = init_env(envp);
	if (!env)
		exit(1);
	while (1)
	{
		user_input_line = readline("λ> ");
		if (ft_streq(user_input_line, "env"))
			printenv(env);
		should_exit_shell = ft_streq(user_input_line, "exit");
		free(user_input_line);
		if (should_exit_shell)
			break ;
	}
	return (0);
}
