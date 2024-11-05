/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:29:36 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/03 10:17:08 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <stdint.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "ast.h"

# ifndef DEBUG
#  define DEBUG 0
# endif

# define NAME "minishell"
# define MINISHELL_PROMPT "λ> "
# define TRUE 1
# define FALSE 0
# define ANSI_RESET "\x1B[0m"

typedef uint8_t	t_bool;

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
char	**make_raw_env_array(t_list *env, t_arena arena);
int		**create_pipes(size_t n_pipes);
void	delete_pipes(int **pipes);
t_list	*get_env_list_location(char *var, t_list **env);
void	unset(char *var, t_list **env);
char	*get_working_dir(void);
int		pwd(t_list **env);
size_t	is_builtin(const char *name);
int		cd(int argc, char **args, t_list **env);
int		echo(char **args);

int		set_signal_handler(void);
int		builtin_exit(char **args);

#endif /* MINISHELL_H */
