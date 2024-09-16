/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:29:36 by copireyr          #+#    #+#             */
/*   Updated: 2024/09/16 11:56:46 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdint.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

# define NAME "minishell"
# define true 1
# define false 0

typedef uint8_t t_bool;

typedef struct s_command
{
	char	*path;
	char	**args;
	char	*infile;
	char	*outfile;
	char	*hdoc_delim;
	t_bool	*of_append;
	int		exit_code;
}	t_command;

typedef struct s_pipeline
{
	t_command	*cmds;
	size_t		n_cmds;
}	t_pipeline;

typedef struct s_children
{
	pid_t	*child_pids;
	size_t	n_children;
}	t_children;

typedef struct s_fds
{
	int		**pipes;
	int		file_fds[2];	
	int		*heredoc_pipe;
}	t_fds;

t_list	**init_env(char **envp);
int		printenv(t_list **env);
char	*get_env(char *var, t_list **env);
int		**create_pipes(size_t n_pipes);
void	delete_pipes(int **pipes, size_t n_pipes);

#endif /* MINISHELL_H */
