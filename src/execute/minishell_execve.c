/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execve.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:27:07 by pleander          #+#    #+#             */
/*   Updated: 2024/11/13 18:29:37 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "minishell.h"

static int	file_isdir(char *path)
{
	struct stat	statbuf;

	stat(path, &statbuf);
	if ((statbuf.st_mode & S_IFMT) == S_IFDIR)
		return (1);
	return (0);
}

static int	executable_file(char *path)
{
	struct stat	statbuf;

	stat(path, &statbuf);
	if ((statbuf.st_mode & S_IFMT) == S_IFREG)
		return (1);
	return (0);
}

int	minishell_execve(char *command, char **args, char **env)
{
	int	i;
	int	last_command_exit_value;

	execve(command, args, env);
	last_command_exit_value = 0;
	i = errno;
	if (i != ENOEXEC)
	{
		if (i == ENOENT)
			last_command_exit_value = EX_NOTFOUND;
		else
			last_command_exit_value = EX_NOEXEC;
	}
	if (file_isdir(command))
		dprintf(STDERR_FILENO, "%s: %s: %s\n", NAME, command, strerror(EISDIR));
	else if (executable_file(command))
		dprintf(STDERR_FILENO, "%s: %s: %s\n", NAME, command, strerror(i));
	else
		dprintf(STDERR_FILENO, "%s: %s: %s\n", NAME, command, strerror(i));
	return (last_command_exit_value);
}
