/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 12:54:11 by tjehaes           #+#    #+#             */
/*   Updated: 2025/02/07 14:46:04 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <curses.h>
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

typedef struct s_env
{
	char	**variables;
	int		exit_status;
	int		size;
}			t_env;

typedef struct s_pipe_info
{
	int		fd_in;
	int		pipefd[2];
	int		is_last_command;
	pid_t	pid;
	int		status;
}			t_pipe_info;

extern sig_atomic_t	g_sigint_received;

# define BLUE "\033[1;34m"
# define RESET "\033[0m"

void		printnb(int nb);
void		syntax_error(char *inpt);
void		execute_lsl(void);
void		execute_ls(void);
void		execute_pwd(t_env *env, char *command);
void		execute_echo(t_env *env, char *inpt);
void		execute_sleep(char *inpt);
void		execute_cat(char *inpt);
void		execute_rm(char *inpt);
void		execute_touch(char *inpt);
void		execute_exit(t_env *env, char *inpt);
void		execute_unset(t_env *env, char *inpt);
void		execute_cd(t_env *env, char *inpt);
void		execute_env(t_env *env, char *command);
void		execute_export(t_env *env, char *inpt);
void		verify_path_order(char *path);
char		*get_env_value(t_env *env, const char *var);
char		*cleanup_string(t_env *env, char *str);
void		handle_sigint(int sig);
void		handle_sigquit(int sig);
char		**pipe_tokenizer(char *command, int *num_commands);
void		execute_pipeline(t_env *env, char **commands, int num_commands);
char		*handle_dollar(t_env *env, char *command);
void		ft_checker(t_env *env, char *command);
int			handle_quote(t_env *env, char *inpt);
bool		is_pipe_inside_quotes(const char *str);
void		skip_whitespace(char *input, int *i);
int			extract_varname(char *input, char **varname, int *i);
void		extract_val(char *input, char **value, int *i);
int			check_empty_functions(char *inpt);

// int				get_env_size(void);
int			skip_spaces(char *input, int i);
char		*int_to_str(int num, char *str);
void		exec_func(t_env *env, char *path, char **args);

// init_shell
void		init_shell(t_env *env, int *saved_stdin, int *saved_stdout);
void		init_struct(t_env *env);
void		free_struct(t_env *env);

// handle_command
int			correct_command(t_env *env, char **args);
char		*command_path(t_env *env, char *command_name);
void		execute_command(t_env *env, char *path, char **args);
bool		is_path_set(t_env *env);

// handle_command2
void		handle_command_error(char **args);
void		handle_access_error(char **args);
void		handle_fork_error(void);
void		handle_external_command(t_env *env, char *command);

// handle_command3
void		exec_perror(t_env *env, char *str);
void		update_exit_status(t_env *env, int status);
void		handle_ls_command(char **args);

// pipe_utils
void		pid_check(pid_t pid);
void		pipe_check(int *pipefd);
void		exit_perror(const char *msg);
int			check_multiple_pipe(t_env *env, char *inpt);
// process_commmand
void		process_command(t_env *env, char *command);

// execute_command
void		execute_commands(t_env *env, char *inpt);

// redirections.c
void		handle_output_redirection(char *filename, bool append);
void		handle_heredoc_redirection(const char *delimiter, t_env *env);
void		handle_input_redirection(char *filename, t_env *env);
void		execute_command_with_redirection(t_env *env, char *cmd, char *args);
void		handle_command_or_args(char *token, char **cmd, char **args);
void		handle_token(t_env *env, char *token, char **cmd, char **args);
void		execute_redirection(t_env *env, char *command);
bool		is_redirection_inside_quotes(const char *str);
void		child_process_heredoc(const char *delimiter,
				int pipefd[2], t_env *env);
void		read_and_write_heredoc(const char *delimiter,
				int pipefd[2], t_env *env);

// tokenizer.c
char		*ft_strtok(char *str, const char *delimiters);

// utils.c
int			ft_strcmp(const char *str1, const char *str2);
int			handle_backspace(int count, int key);
int			handle_tab(int count, int key);
int			is_line_empty(const char *line);
int			check_quote_empty(char *line);
int			count_arg(char *inpt);
int			check_check(char *line);

// export_setenv.c
char		*create_new_var(const char *name, const char *value);
int			update_existing_var(t_env *env, int index, const char *args[],
				int overwrite);
int			custom_setenv(t_env *env, const char *name, const char *value,
				int overwrite);

// export_handle_new_var.c
char		**allocate_new_variables(int size);
char		*create_variable_string(const char *varname, const char *value);
void		handle_new_var(t_env *env, char *varname, char *value);
void		print_exported_variables(t_env *env);

#endif
