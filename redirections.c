#include "minishell.h"

bool	is_redirection_inside_quotes(const char *str)
{
	bool	in_single_quote;
	bool	in_double_quote;

	in_single_quote = false;
	in_double_quote = false;
	while (*str)
	{
		if (*str == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*str == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (*str == '<' && !in_single_quote && !in_double_quote)
			return (false);
		else if (*str == '>' && !in_single_quote && !in_double_quote)
			return (false);
		str++;
	}
	return (true);
}

// Function to handle input redirection (<)
void	handle_input_redirection(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return ;
	}
	if (dup2(fd, STDIN_FILENO) == -1)
		// duplicate the fd to STDIN_FILENO: redirects stdin to read from the specified file
	{
		perror("dup2");
		close(fd);
		return ;
	}
	close(fd);
}

void	handle_output_redirection(char *filename, bool append)
{
	int	fd;

	while (*filename == ' ' || *filename == '\t')
		filename++;
	if (append)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		return ;
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return ;
	}
	close(fd);
}

void	handle_heredoc_redirection(const char *delimiter)
{
	char	*line;
	int		pipefd[2];

	line = NULL;
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return ;
	}
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			perror("readline");
			break ;
		}
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
}
void	handle_command_or_args(char *token, char **cmd, char **args)
{
	char	*temp;
	char	*new_args;

	if (*cmd == NULL)
	{
		*cmd = ft_strdup(token);
	}
	else
	{
		if (*args == NULL)
			*args = ft_strdup(token);
		else
		{
			temp = ft_strjoin(*args, " ");
			new_args = ft_strjoin(temp, token);
			free(temp);
			free(*args);
			*args = new_args;
		}
	}
}
void	handle_token(char *token, char **cmd, char **args)
{
	if (ft_strcmp(token, ">") == 0)
	{
		token = ft_strtok(NULL, " ");
		if (token != NULL)
			handle_output_redirection(token, false);
	}
	else if (ft_strcmp(token, ">>") == 0)
	{
		token = ft_strtok(NULL, " ");
		if (token != NULL)
			handle_output_redirection(token, true);
	}
	else if (ft_strcmp(token, "<") == 0)
	{
		token = ft_strtok(NULL, " ");
		if (token != NULL)
			handle_input_redirection(token);
	}
	else if (ft_strcmp(token, "<<") == 0)
	{
		token = ft_strtok(NULL, " ");
		if (token != NULL)
			handle_heredoc_redirection(token);
	}
	else
		handle_command_or_args(token, cmd, args);
}
void	execute_command_with_redirection(char *cmd, char *args)
{
	char	*full_command;

	if (cmd != NULL)
	{
		size_t full_command_length = ft_strlen(cmd)
			+ (args ? ft_strlen(args) : 0) + 2;
			// checks if args is not null(if so return 0),then adds the length of args to the length of cmd
		full_command = malloc(full_command_length);
		ft_strcpy(full_command, cmd);
		if (args)
		{
			ft_strlcat(full_command, " ", full_command_length);
			ft_strlcat(full_command, args, full_command_length);
		}
		ft_checker(full_command);
		free(full_command);
		free(cmd);
		if (args)
			free(args);
	}
}
void	execute_redirection(char *command)
{
	char *token;
	char *cmd = NULL;
	char *args = NULL;

	token = ft_strtok(command, " ");
	while (token != NULL)
	{
		handle_token(token, &cmd, &args);
		token = ft_strtok(NULL, " ");
	}
	if (cmd != NULL)
		if (ftstrchr(cmd, '|') && !is_pipe_inside_quotes(cmd))
		{
			int num_commands;
			char **commands = pipe_tokenizer(cmd, &num_commands);
			if (commands)
			{
				execute_pipeline(commands, num_commands);
				for (int i = 0; i < num_commands; i++)
					free(commands[i]);
				free(commands);
			}
		}
		else
			execute_command_with_redirection(cmd, args);
	else
		printf("Error: No valid command found to execute.\n");
}
