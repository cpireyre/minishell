/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 10:14:24 by pleander          #+#    #+#             */
/*   Updated: 2024/10/29 15:36:33 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static int run_builtin_pwd()
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

	ret = export(args[0], env); // Should we check that there's only one argument?
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

static int	run_builtin_cd(char **args)
{
	int ret;

	ret = cd(args[1]);
	if (ret)
		perror(NAME);
	return (ret);
}

int	run_builtin(char *builtin, char **args, t_list **env)
{
	if (ft_streq(builtin, "env"))
		return (run_builtin_env(env));
	if (ft_streq(builtin, "export"))
		return(run_builtin_export(args, env));
	if (ft_streq(builtin, "unset"))
		return(run_builtin_unset(args, env));
	if (ft_streq(builtin, "pwd"))
		return(run_builtin_pwd());
	if (ft_streq(builtin, "cd"))
		return(run_builtin_cd(args));
	ft_dprintf(2, "%s", "Error: No such builtin");
	return (1);
}
