/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 10:14:24 by pleander          #+#    #+#             */
/*   Updated: 2024/11/08 09:30:01 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static int	run_builtin_pwd(t_list **env)
{
	int	ret;

	ret = pwd(env);
	return (ret);
}

static int	run_builtin_env(t_list **env)
{
	int	ret;

	ret = printenv(env);
	return (ret);
}

static int	run_builtin_export(char **args, t_list **env)
{
	int	ret;

	if (args[1])
		ret = export(args[1], env);
	else
		ret = list_exports(*env);
	return (ret);
}

/* Can unset fail? */
static int	run_builtin_unset(char	**args, t_list **env)
{
	size_t	i;

	i = 0;
	while (args[i])
	{
		unset(args[i], env);
		i++;
	}
	return (0);
}

t_shell_status	run_builtin(
		char *builtin, char **args, t_list **env, int prev_exit)
{
	if (DEBUG)
		ft_printf("Running builtin %s\n", builtin);
	if (!builtin)
		return ((t_shell_status){.exit_code = 0});
	if (ft_streq(builtin, "env"))
		return ((t_shell_status){.exit_code = run_builtin_env(env)});
	if (ft_streq(builtin, "export"))
		return ((t_shell_status){.exit_code = run_builtin_export(args, env)});
	if (ft_streq(builtin, "unset"))
		return ((t_shell_status){.exit_code = run_builtin_unset(args, env)});
	if (ft_streq(builtin, "pwd"))
		return ((t_shell_status){.exit_code = run_builtin_pwd(env)});
	if (ft_streq(builtin, "cd"))
		return ((t_shell_status){.exit_code = cd(args, env)});
	if (ft_streq(builtin, "echo"))
		return ((t_shell_status){.exit_code = echo(args)});
	if (ft_streq(builtin, "exit"))
		return (builtin_exit(args, prev_exit));
	return ((t_shell_status){.exit_code = 1});
}
