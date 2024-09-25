/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:08:02 by copireyr          #+#    #+#             */
/*   Updated: 2024/09/25 10:53:53 by pleander         ###   ########.fr       */
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
		char **input = ft_split(user_input_line, ' ');
		free(user_input_line);
		if (ft_streq(input[0], "env"))
			printenv(env);
		else if (ft_streq(input[0], "export"))
			export(input[1], env);
		else if (ft_streq(input[0], "unset"))
			unset(input[1], env);
		should_exit_shell = ft_streq(input[0], "exit");

		char **orig = input;
		while (*input)
		{
			free(*input);
			input++;
		}
		free(orig);
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
