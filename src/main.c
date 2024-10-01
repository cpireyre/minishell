/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:08:02 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/01 09:40:53 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "tokenize.h"
#include "libft.h"

int	main(int argc, char **argv, char **envp)
{
	t_list	**env;
	t_token	*xs;
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
		should_exit_shell = ft_streq(user_input_line, "exit");
		if (!user_input_line)
			break ;
		xs = tokenize(user_input_line);
		if (xs)
		{
			parse(xs);
			free(xs);
		}
		if (ft_streq(user_input_line, "env"))
			printenv(env);
		should_exit_shell = ft_streq(user_input_line, "exit");
		free(user_input_line);
		char	**input = ft_split(user_input_line, ' ');
		if (ft_streq(input[0], "env"))
			printenv(env);
		else if (ft_streq(input[0], "export"))
			export(input[1], env);
		else if (ft_streq(input[0], "unset"))
			unset(input[1], env);

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
