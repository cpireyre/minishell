/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:28:45 by pleander          #+#    #+#             */
/*   Updated: 2024/11/07 15:42:19 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "minishell.h"

static int	handle_tilde(char *path, t_list **env)
{
	char	*home_path;
	char	*expanded_path;

	home_path = get_env("HOME", env);
	if (!home_path)
	{
		ft_dprintf(2, "%s: cd: HOME not set\n", NAME);
		return (1);
	}
	expanded_path = ft_strjoin(home_path, &path[1]);
	if (!expanded_path)
		return (1);
	if (chdir(expanded_path) < 0)
	{
		ft_dprintf(2, "%s: cd: ", NAME);
		perror(expanded_path);
		free(expanded_path);
		return (1);
	}
	if (set_working_dir("OLDPWD", env) < 0)
		return (1);
	free(expanded_path);
	return (0);
}

static int	handle_dash(t_list **env)
{
	char	*old_pwd;

	old_pwd = get_env("OLDPWD", env);
	if (!old_pwd)
	{
		ft_dprintf(2, "%s: cd: OLDPWD not set\n", NAME);
		return (1);
	}
	old_pwd = ft_strdup(old_pwd);
	if (!old_pwd)
		return (1);
	if (chdir(old_pwd) < 0)
	{
		ft_dprintf(2, "%s: cd: ", NAME);
		perror(old_pwd);
		free(old_pwd);
		return (1);
	}
	free(old_pwd);
	if (set_working_dir("OLDPWD", env) < 0)
		return (1);
	return (0);
}

static int	handle_no_args(t_list **env)
{
	char	*home_path;

	home_path = get_env("HOME", env);
	if (!home_path)
	{
		ft_dprintf(2, "%s: cd: HOME not set\n", NAME);
		return (1);
	}
	if (chdir(home_path) < 0)
	{
		ft_dprintf(2, "%s: cd: ", NAME);
		perror(home_path);
		return (1);
	}
	if (set_working_dir("OLDPWD", env) < 0)
		return (1);
	return (0);
}

static int	handle_path(char *path)
{
	if (chdir(path) < 0)
	{
		ft_dprintf(2, "%s: cd: ", NAME);
		perror(path);
		return (1);
	}
	return (0);
}

int	cd(char **args, t_list **env)
{
	int		ret;
	char	*old_dir;
	int		argc;

	old_dir = get_working_dir();
	if (!old_dir)
		return (1);
	argc = 0;
	while (args[argc])
		argc++;
	if (argc == 1)
		ret = handle_no_args(env);
	else if (args[1][0] == '~')
		ret = handle_tilde(args[1], env);
	else if (ft_streq(args[1], "-"))
		ret = handle_dash(env);
	else
		ret = handle_path(args[1]);
	if (ret == 0)
		ret = (int)(set_env("OLDPWD", old_dir, env) < 0)
			| (int)(set_working_dir("PWD", env) < 0);
	free(old_dir);
	if (argc > 2)
		return (!!ft_dprintf(2, "%s: cd: too many arguments\n", NAME));
	return (!!ret);
}
