/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 12:54:11 by tjehaes           #+#    #+#             */
/*   Updated: 2024/11/12 11:47:55 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <curses.h>
#include <term.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <termios.h>
#include <sys/wait.h>
#include "libft/libft.h"


#define BLUE "\033[1;34m"
#define RESET "\033[0m"

/*To hold key-value data, like environment var in a linked list
//exmp. t_list *node1 = malloc(sizeof(t_list));
node1->key = strdup("PATH");
node1->value = strdup("/usr/bin:/bin");
node1->next = NULL;*/

typedef struct s_list
{
	char			*key;
	char			*value;
	struct s_list	*next;
}	t_list;

/*holds command-related data, file descriptors, and environment variables*/


typedef struct s_data
{
	char	**command;
	int		is_pipe;
	int		*pipe_locale; //for the locaation
	int		in_out_fd[6];
	int		command_num; //for the number of commands
	int		initial_commands;
	t_list	**table;
	// struct sigaction for sigint;
	// struct sigaction for sigquit;
}	t_data;

int		exit_status;

void    printnb(int nb);
void    syntax_error(char *inpt);
void    execute_lsl();
void	execute_ls();
void	execute_pwd(char *command);
void    execute_echo(char *inpt);
void    execute_sleep(char *inpt);
void	execute_cat(char *inpt);
void	execute_rm(char *inpt);
void	execute_touch(char *inpt);
void	execute_exit(char *inpt);
void 	execute_unset(char *inpt);
void    execute_cd(char *inpt);
void    execute_env(char *command);
void    execute_export(char *inpt);
char	*execute_pwdmain(void);
char	*get_env_value(const char *var);
char	*cleanup_string(char *str);
void	handle_sigint(int sig);
char **pipe_tokenizer(char *command, int *num_commands);
void 	execute_pipeline(char **commands, int num_commands);
char	*handle_dollar(char *command);
void	ft_checker(char *command);
int    	handle_quote(char *inpt);
bool 	is_pipe_inside_quotes(const char *str);

// redirections.c
void 	handle_output_redirection(char *filename, bool append);
void 	handle_heredoc_redirection(const char *delimiter);
void 	handle_input_redirection(char *filename);
void 	execute_command_with_redirection(char *cmd, char *args);
void 	handle_token(char *token, char **cmd, char **args);
void 	execute_redirection(char *command);
const char *identify_redirection_operator(const char *str);
bool 	is_redirection_inside_quotes(const char *str);

// tokenizer.c
char	*ft_strtok(char *str, const char *delimiters);

// utils.c
int		ft_strcmp(const char *str1, const char *str2);

#endif