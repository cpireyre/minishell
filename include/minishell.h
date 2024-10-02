/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:29:36 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/02 12:17:58 by copireyr         ###   ########.fr       */
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
# define MINISHELL_PROMPT "λ> "
# define TRUE 1
# define FALSE 0
# define ANSI_RESET "\x1B[0m"

typedef uint8_t	t_bool;

typedef struct s_command
{
	size_t	i;
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
int		export(char *export_str, t_list **env);
int		printenv(t_list **env);
char	*get_env(char *var, t_list **env);
int		set_env(char *var, char *val, t_list **env);
int		**create_pipes(size_t n_pipes);
void	delete_pipes(int **pipes);
int		pipeline(t_pipeline *pipeline, char **env);
void	spawn_child(t_command *cmd, int **pipes, char **env);
t_list	*get_env_list_location(char *var, t_list **env);
void	unset(char *var, t_list **env);

int		set_signal_handler(void);

#endif /* MINISHELL_H */
