/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 10:14:24 by pleander          #+#    #+#             */
/*   Updated: 2024/10/29 12:44:53 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static void run_builtin_pwd()
{
	int	ret;

	ret = pwd();
	exit(ret);
}

static void	run_builtin_env(t_list **env)
{
	int ret;

	ret = printenv(env);
	exit(ret);
}

static void	run_builtin_export(char **args, t_list **env)
{
	int ret;

	ret = export(args[0], env); // Should we check that there's only one argument?
	exit(ret);
}

static	void run_builtin_unset(char	**args, t_list **env)
{
	size_t	i;
	
	i = 0;
	while (args[i])
	{
		unset(args[i], env); // can it fail?
		i++;
	}
	exit(0);
}

void	run_builtin(char *builtin, char **args, t_list **env)
{
	if (ft_streq(builtin, "env"))
		run_builtin_env(env);
	if (ft_streq(builtin, "export"))
		run_builtin_export(args, env);
	if (ft_streq(builtin, "unset"))
		run_builtin_unset(args, env);
	if (ft_streq(builtin, "pwd"))
		run_builtin_pwd();
	ft_dprintf(2, "%s", "Error: No such builtin");
	exit(1);
}
