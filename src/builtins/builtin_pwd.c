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

#include <unistd.h>
#include "libft.h"
#include "minishell.h"

int	set_working_dir(char *var, t_list **env)
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

char	*get_working_dir(void)
{
	size_t	buf_size;
	char	*buf;

	buf_size = 255;
	buf = malloc(buf_size);
	if (!buf)
		return (NULL);
	getcwd(buf, buf_size);
	while (!buf)
	{
		free(buf);
		buf_size += 255;
		buf = malloc(buf_size);
		if (!buf)
			return (NULL);
		getcwd(buf, buf_size);
	}
	return (buf);
}

int	pwd(t_list **env)
{
	const char	*pwd_value = get_env("PWD", env);

	if (!pwd_value)
		return (1);
	ft_printf("%s\n", pwd_value);
	return (0);
}
