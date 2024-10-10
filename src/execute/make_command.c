/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:09:05 by pleander          #+#    #+#             */
/*   Updated: 2024/10/10 17:22:46 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "execute.h"
#include "arena.h"
#include "ast.h"
#include "get_next_line.h"
#include "libft.h"
#include "minishell.h"


int		count_cmd_args(t_ast_node *ast)
{
	int c;
	size_t i;

	c = 0;
	i = 0;
	while (i < ast->n_children)
	{
		if (ast->children[i]->type == AST_WORD)
			c++;
		i++;
	}
	return (c);
}

static char	*build_path(char *path, const char *command, t_arena arena)
{
	int		cmdlen;
	int		pathlen;
	char	*cmd;

	pathlen = ft_strchrnul(path, ':') - path;

	cmdlen = ft_strlen(command);
	cmd = arena_calloc(arena, pathlen + 1 + cmdlen + 1, sizeof(char));
	if (!cmd)
		return (NULL);
	ft_memcpy(cmd, path, pathlen);
	cmd[pathlen] = '/';
	ft_memcpy(cmd + pathlen + 1, command, cmdlen);
	return (cmd);
}

static char	*find_path(const char *command, t_list *env, t_arena arena)
{
	char	*path;
	char	*exec_path;
	char	*split_path;
	int		i;

	exec_path = get_env("PATH", &env);
	while (exec_path)
	{
		if (*exec_path == ':')
			exec_path++;
		path = build_path(exec_path, command, arena);
		if (!path)
			return (NULL);
		if (access(path, F_OK) == 0)
			return (path);
		exec_path = ft_strchr(exec_path, ':');
	}
	path = ft_arena_strndup(arena, command, ft_strlen(command) + 1);
	if (!path)
		return (NULL);
	return (path);
}

int	make_command(t_command *cmd, t_ast_node *ast, t_list *env, t_arena arena)
{
	cmd->args = arena_calloc(arena, count_cmd_args(ast), sizeof(char *));
	if (!cmd->args)
		return (-1);
	cmd->path = find_path(ast->children[0]->token.value, env, arena);
	ft_printf("Full path is %s\n", cmd->path);

	// TODO: copy commands to cmd
	// Open file descriptors and store them in cmd
	// Read heredoc
	return (0);
}
