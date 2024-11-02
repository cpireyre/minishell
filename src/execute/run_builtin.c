/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 10:14:24 by pleander          #+#    #+#             */
/*   Updated: 2024/10/30 14:55:11 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static int run_builtin_pwd(void)
{
	int	ret;

	ret = pwd();
	return (ret);
}

static int	run_builtin_env(t_list **env)
{
	int ret;

	ret = printenv(env);
	return (ret);
}

static int	run_builtin_export(char **args, t_list **env)
{
	int ret;

	ret = export(args[1], env); // Should we check that there's only one argument?
	return (ret);
}

static	int run_builtin_unset(char	**args, t_list **env)
{
	size_t	i;
	
	i = 0;
	while (args[i])
	{
		unset(args[i], env); // can it fail?
		i++;
	}
	return (0);
}

static int	run_builtin_cd(int argc, char **args, t_list **env)
{
	int ret;

	if (argc > 2)
	{
		ft_dprintf(2, "%s: cd: too many arguments\n", NAME);
		return (1);
	}
	ret = cd(argc, args, env);
	return (ret);
}

static int	run_builtin_echo(char **args)
{
	int	ret;

	ret = echo(args);
	return (ret);
}

static int count_args(char **args)
{
	int	c;

	c = 0;
	while (args[c])
		c++;
	return (c);
}

int	run_builtin(char *builtin, char **args, t_list **env)
{
	if (DEBUG)
		ft_printf("Running builtin %s\n", builtin);
	if (ft_streq(builtin, "env"))
		return (run_builtin_env(env));
	if (ft_streq(builtin, "export"))
		return (run_builtin_export(args, env));
	if (ft_streq(builtin, "unset"))
		return (run_builtin_unset(args, env));
	if (ft_streq(builtin, "pwd"))
		return (run_builtin_pwd());
	if (ft_streq(builtin, "cd"))
		return (run_builtin_cd(count_args(args), args, env));
	if (ft_streq(builtin, "echo"))
		return (run_builtin_echo(args));
	if (ft_streq(builtin, "exit"))
		return(0);
	return (1);
}
