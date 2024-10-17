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

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "execute.h"
#include "arena.h"
#include "ast.h"
#include "get_next_line.h"
#include "libft.h"
#include "minishell.h"
#include "tokenize.h"


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

static void	close_fd_if_open(int *fd)
{
	if (*fd != -1)
		close(*fd);
	*fd = -1;
}

int handle_redir(t_command *cmd, t_ast_node *ast)
{

	if (ast->type != AST_REDIR || ast->children[0]->type != AST_WORD)
		return (-1);
	if (ast->token.type == TOK_REDIRECT_IN)
	{
		close_fd_if_open(&cmd->infile);
		cmd->infile = open(ast->children[0]->token.value, O_RDONLY);
		if (cmd->infile < 0)
		{
			perror(NAME);
			return (-1);
		}
	}
	else if (ast->token.type == TOK_REDIRECT_OUT)
	{
		close_fd_if_open(&cmd->infile);
		cmd->outfile = open(ast->children[0]->token.value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (cmd->outfile < 0)
		{
			perror(NAME);
			return (-1);
		}
	}
	else if (ast->token.type == TOK_APPEND)
	{
		close_fd_if_open(&cmd->infile);
		cmd->outfile = open(ast->children[0]->token.value, O_WRONLY | O_CREAT, 0644);
		{
			perror(NAME);
			if (cmd->outfile < 0)
				return (-1);
		}
	}
	else
	{
		ft_dprintf(2, "Error: token not supported yet\n");
		return (-1);
	}
	return (0);
}

static int parse_children(t_command *cmd, t_ast_node *ast, t_list *env, t_arena arena)
{
	size_t i;
	size_t arg_i;

	i = 0;
	arg_i = 0;
	while (i < ast->n_children)
	{
		if (ast->children[i]->type == AST_WORD)
		{
			if (cmd->path == NULL)
			{
				cmd->path = find_path(ast->children[i]->token.value, env, arena);
				if (!cmd->path)
					return (-1);
				cmd->args[arg_i] = cmd->path;
			}
			else
				cmd->args[arg_i] = ast->children[i]->token.value;
			arg_i++;
		}
		else if (ast->children[i]->type == AST_REDIR)
		{
			if (handle_redir(cmd, ast->children[i]) < 0)
				return (-1);
		}
		i++;
	}
	return (0);
}

static void	print_cmd(t_command *cmd)
{
	size_t i;

	ft_printf("Command: %s\nArgs: ", cmd->path);
	i = 0;
	while (cmd->args[i])
	{
		ft_printf("%s, ", cmd->args[i]);
		i++;
	}
	ft_printf("\n");
	if (cmd->infile > -1)
		ft_printf("Infile opened on fd: %d\n", cmd->infile);
	if (cmd->outfile > -1)
		ft_printf("Outfile opened on fd: %d\n", cmd->outfile);
}

int	make_command(t_command *cmd, t_ast_node *ast, t_list *env, t_arena arena)
{
	if (ast->type != AST_COMMAND)
	{
		ft_dprintf(2, "Error: Wrong AST type\n");
		return (-1);
	}
	cmd->infile = -1;
	cmd->outfile = -1;
	cmd->path = NULL;
	cmd->args = arena_calloc(arena, count_cmd_args(ast) + 1, sizeof(char *));
	if (!cmd->args)
		return (-1);
	if (parse_children(cmd, ast, env, arena) < 0)
		return (-1);
	print_cmd(cmd);
	// TODO
	// Read heredoc
	return (0);
}
