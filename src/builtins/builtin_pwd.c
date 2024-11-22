/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 12:31:14 by pleander          #+#    #+#             */
/*   Updated: 2024/11/05 21:47:30 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <linux/limits.h>
#include <unistd.h>
#include "libft.h"
#include "minishell.h"

int	set_working_dir(char *var, t_list **env)
{
	char	path[PATH_MAX];
	int		ret;

	if (!get_working_dir(path, SIZE_MAX, env))
		return (-1);
	ret = set_env(var, path, env);
	if (ret < 0)
		return (-1);
	return (0);
}

char	*get_working_dir(char *path, size_t path_size, t_list **env)
{
	char	*tmp;

	if (getcwd(path, PATH_MAX) == NULL)
	{
		ft_dprintf(2, "cd: error retrieving current directory\n");
		tmp = get_env("PWD", env);
		if (!tmp)
		{
			perror(NAME);
			return (NULL);
		}
		ft_strlcpy(path, tmp, path_size);
	}
	return (path);
}

int	pwd(t_list **env)
{
	const char	*pwd_value = get_env("PWD", env);

	if (!pwd_value)
		return (1);
	ft_printf("%s\n", pwd_value);
	return (0);
}
