/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:03:23 by pleander          #+#    #+#             */
/*   Updated: 2024/11/20 13:46:17 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "ast.h"

typedef struct s_command
{
	char	*path;
	char	**args;
	char	*infile;
	char	*outfile;
	int		infile_fd;
	int		outfile_fd;

}	t_command;

typedef struct s_command_context
{
	t_command	cmd;
	t_ast_node	*ast;
	t_list		*env;
	int			**pipes;
	size_t		cur_child;
	size_t		n_children;
	int			child_should_exit;
	bool		should_quit_pipeline;
}	t_command_context;

typedef struct s_shell_status
{
	bool	should_exit;
	bool	sigint_received;
	int		exit_code;
}	t_shell_status;

t_shell_status	execute_ast(t_ast_node *ast, t_list *env, t_arena arena,
					int prev_exit);
int				make_command(t_command *cmd, t_ast_node *ast, t_list *env,
					t_arena ar);
int				**make_pipes(int n_pipes, t_arena arena);
void			close_pipes(int **pipes, int n_pipes);
size_t			calculate_n_pipes(t_ast_node *ast);
t_shell_status	run_builtin(char *builtin, char **args, t_list **env,
					int prev_exit);
t_shell_status	builtin_exit(char **args, int prev_exit);
t_shell_status	execute_logicals(t_ast_node *ast, t_list *env, t_arena arena,
					int prev_exit);
int				minishell_execve(char *command, char **args, char **env);
int				is_path(char *command);
void			print_command(t_command *cmd);
int				execute_cmd(t_command *cmd, t_command_context *con,
					t_arena arena, int prev_exit);
int				wait_for_children(int *pid, size_t n_forks);
t_shell_status	execute_single_command(t_ast_node *ast, t_list *env,
					t_arena arena, int prev_exit);
t_shell_status	execute_pipeline(t_ast_node *ast, t_list *env, t_arena arena,
					int prev_exit);
void			close_fd_if_open(int *fd);
int				count_cmd_args(t_ast_node *ast);
char			*find_path(const char *command, t_list *env, t_arena arena);
int				handle_redir(t_command *cmd, t_ast_node *ast);
int				get_exit_code(int wstatus);

#endif
