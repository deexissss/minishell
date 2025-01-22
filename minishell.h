/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 12:54:11 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/15 09:30:03 by tjehaes          ###   ########.fr       */
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

//extern int		g_exit_status;
typedef struct s_env
{
	char		**variables;
	int			exit_status;
	int			size;
}				t_env;

extern t_env	g_env;

# define BLUE "\033[1;34m"
# define RESET "\033[0m"

void			printnb(int nb);
void			syntax_error(char *inpt);
void			execute_lsl(void);
void			execute_ls(void);
void			execute_pwd(char *command);
void			execute_echo(char *inpt);
void			execute_sleep(char *inpt);
void			execute_cat(char *inpt);
void			execute_rm(char *inpt);
void			execute_touch(char *inpt);
void			execute_exit(char *inpt);
void			execute_unset(char *inpt);
void			execute_cd(char *inpt);
void			execute_env(char *command);
void			execute_export(char *inpt);
void			verify_path_order(char *path);
char			*execute_pwdmain(void);
char			*get_env_value(const char *var);
char			*cleanup_string(char *str);
void			handle_sigint(int sig);
void			handle_sigquit(int sig);
char			**pipe_tokenizer(char *command, int *num_commands);
void			execute_pipeline(char **commands, int num_commands);
char			*handle_dollar(char *command);
void			ft_checker(char *command);
int				handle_quote(char *inpt);
bool			is_pipe_inside_quotes(const char *str);
void			skip_whitespace(char *input, int *i);
int				extract_varname(char *input, char **varname, int *i);
void			extract_val(char *input, char **value, int *i);
int				get_env_size(void);
int				skip_spaces(char *input, int i);
char			*int_to_str(int num, char *str);
void			exec_func(char *path, char **args);
// handle_command

int				correct_command(char **args);
char			*command_path(char *command_name);
void			execute_command(char *path, char **args);
bool			is_path_set(void);

// handle_command2
void			handle_command_error(char **args);
void			handle_access_error(char **args);
void			handle_fork_error(void);
void			handle_external_command(char *command);

// pipe_utils
void			pid_check(pid_t pid);
void			pipe_check(int *pipefd);
void			exit_perror(const char *msg);
int				check_multiple_pipe(char *inpt);
// process_commmand
void			process_command(char *command);

// execute_command
void			execute_commands(char *inpt);

// redirections.c
void			handle_output_redirection(char *filename, bool append);
void			handle_heredoc_redirection(const char *delimiter);
void			handle_input_redirection(char *filename);
void			execute_command_with_redirection(char *cmd, char *args);
void			handle_command_or_args(char *token, char **cmd, char **args);
void			handle_token(char *token, char **cmd, char **args);
void			execute_redirection(char *command);
bool			is_redirection_inside_quotes(const char *str);

// tokenizer.c
char			*ft_strtok(char *str, const char *delimiters);

// utils.c
int				ft_strcmp(const char *str1, const char *str2);
int				handle_backspace(int count, int key);
int				handle_tab(int count, int key);

#endif
