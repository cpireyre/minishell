/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:28:45 by pleander          #+#    #+#             */
/*   Updated: 2024/11/05 21:07:18 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "minishell.h"

static int	set_working_dir(char *var, t_list **env)
{
	char	*temp;
	int		ret;

	temp = get_working_dir();
	if (!temp)
		return (-1);
	ret = set_env(var, temp, env);
	free(temp);
	if (ret < 0)
		return (-1);
	return (0);
}

static int	handle_tilde(char *path, t_list **env)
{
	char	*home_path;
	char	*expanded_path;
	int		ret;

	home_path = get_env("HOME", env);
	if (!home_path)
		return (1);
	expanded_path = ft_strjoin(home_path, &path[1]);
	if (!expanded_path)
		return (1);
	if (set_working_dir("OLDPWD", env) < 0)
		return (1);
	if (chdir(expanded_path) < 0)
	{
		ft_dprintf(2, "%s: cd: ", NAME);
		perror(expanded_path);
		free(expanded_path);
		return (1);
	}
	free(expanded_path);
	return (0);
}

static int	handle_dash(t_list **env)
{
	char	*old_pwd;
	int		ret;

	old_pwd = get_env("OLDPWD", env);
	if (!old_pwd)
		return (1);
	old_pwd = ft_strdup(old_pwd);
	if (!old_pwd)
		return (1);
	if (set_working_dir("OLDPWD", env) < 0)
		return (1);
	ret = chdir(old_pwd);
	free(old_pwd);
	return (ret);
}

static int	handle_no_args(t_list **env)
{
	char	*home_path;
	int		ret;

	home_path = get_env("HOME", env);
	if (!home_path)
		return (1);
	if (set_working_dir("OLDPWD", env) < 0)
		return (1);
	ret = chdir(home_path);
	return (0);
}

int	cd(int argc, char **args, t_list **env)
{
	int		ret;

	ret = 1;
	if (argc == 1)
		ret = handle_no_args(env);
	else if (args[1][0] == '~')
		ret = handle_tilde(args[1], env);
	else if (ft_streq(args[1], "-"))
		ret = handle_dash(env);
	else
	{
		if (set_working_dir("OLDPWD", env) < 0)
			return (1);
		if (chdir(args[1]) < 0)
		{
			ft_dprintf(2, "%s", NAME ": cd: ");
			perror(args[1]);
		}
		else
			ret = 0;
	}
	if (set_working_dir("PWD", env) < 0)
		return (1);
	return (ret);
}
