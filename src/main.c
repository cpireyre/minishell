/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:08:02 by copireyr          #+#    #+#             */
/*   Updated: 2024/09/24 15:30:16 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "tokenize.h"
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
		if (!user_input_line)
			break ;
		tokenize(user_input_line);
		printf("user input line is: %s\n", user_input_line);
		if (ft_streq(user_input_line, "env"))
			printenv(env);
		if (ft_strncmp(user_input_line, "export ", ft_strlen("export ") == 0))
		{
			char **var = ft_split(user_input_line, ' ');
			export(var[1], env);
		}
		should_exit_shell = ft_streq(user_input_line, "exit");
		free(user_input_line);
		if (should_exit_shell)
			break ;
	}
	close(0);
	close(1);
	close(2);
	ft_lstclear(env, &free);
	free(env);
	return (0);
}
