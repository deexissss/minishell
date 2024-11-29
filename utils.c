/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg.l    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 09:12:55 by tjehaes           #+#    #+#             */
/*   Updated: 2024/11/07 14:10:09 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    syntax_error(char *inpt)
{
    if (memcmp(inpt, ">", 2) == 0)
        ft_printf("syntax error");
    if (memcmp(inpt, "<", 2) == 0)
        ft_printf("syntax error");
    if (memcmp(inpt, ">>", 2) == 0)
        ft_printf("syntax error");
    if (memcmp(inpt, "<<", 2) == 0)
        ft_printf("syntax error");
    if (memcmp(inpt, "<>", 2) == 0)
        ft_printf("syntax error");
}

void    execute_lsl()
{
    char *args[] = {"/bin/ls", "-l", NULL};
    pid_t pid = fork();

    if (pid == -1)
        perror("fork");
    else if (pid == 0)
    {
        if (execve(args[0], args, NULL) == -1)
        {
            perror("execve");
            _exit(1);
        }
    }
    else
        waitpid(pid, NULL, 0);
}

/*void    execute_ls()
{
    char *args[] = {"/bin/ls", NULL};
    pid_t pid = fork();

    if (pid == -1)
        perror("fork");
    else if (pid == 0)
    {
        if (execve(args[0], args, NULL) == -1)
        {
            perror("execve");
            _exit(1);
        }
    }
    else
        waitpid(pid, NULL, 0);
}*/

void execute_ls()
{
    DIR *dir;
    struct dirent *entry;
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        dir = opendir(".");
        if (dir == NULL)
        {
            perror("opendir");
            exit(1);
        }
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] != '.')
            {
                write(1, entry->d_name, ft_strlen(entry->d_name));
                write(1, "  ", 3);
            }
        }
        closedir(dir);
        exit(0);
    }
    else
        waitpid(pid, NULL, 0);
    printf("\n");
}

void execute_pwd()
{
    char cwd[50000];
    pid_t pid;

    pid = fork();
    if (pid == -1)
        perror("fork");
    else if (pid == 0)
    {
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            write(1, cwd, ft_strlen(cwd));
            write(1, "\n", 1);
        } else
        {
            perror("getcwd");
            exit(1);
        }
        exit(0);
    }
    else
        waitpid(pid, NULL, 0);
}

void    execute_echo(char *inpt)
{
    int     i;
    int     n;//flag to control newline printing

    i = 4;
    n = 1; //newline character will be printed by default
    //skip whitespace
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    //check for -n flag
    //(inpt[i + 2] == ' ' || ...) ensures that the -n flag is a standalone flag and not part of the text to be echoed
    if (inpt[i] == '-' && inpt[i + 1] == 'n' && (inpt[i + 2] == ' ' || 
        inpt[i + 2] == '\t' || inpt[i + 2] == '\0'))
    {
        n = 0;//if detected n set to 0 to disable newline printing
        i += 2;// move index past -n
        //skip any additional whitespace after -n flag
        while(inpt[i] == ' ' || inpt[i] == '\t')
            i++;
    }

    while (inpt[i])
    {
        write(1, &inpt[i], 1);
        i++;
    }
    //conditional newline, if n = 1 print newline, if -n flag was set no newline
    if(n)
        write(1, "\n", 1);
}

void    execute_sleep(char *inpt)
{
    int     i;
    int     time;
    pid_t pid;

    i = 5;
    time = 0;
    pid = fork();
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    if (inpt[i] == '\0')
        perror("sleep");
    while (inpt[i] >= '0' && inpt[i] <= '9')
    {
        time = time * 10 + (inpt[i] - '0');
        i++;
    }
    if (pid == -1)
        perror("fork");
    else if (pid == 0)
    {
        char time_str[20];
        snprintf(time_str, sizeof(time_str), "%d", time);
        char *args[] = {"sleep", time_str, NULL};
        if (execvp(args[0], args) == -1)
        {
            perror("execvp");
            _exit(1);
        }
    }
    else
        waitpid(pid, NULL, 0);
}

void execute_cat(char *inpt)
{
    int     i;
    int     fd;
    char    buffer[42];
    char    filename[70000];
    int bytes_read;
    pid_t   pid;
    int j;

    i = 3;
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    if (inpt[i] == '\0')
    {
        ft_printf("error : cat needs a file name\n");
        return;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        j = 0;
        while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t' && j < 70000)
            filename[j++] = inpt[i++];
        filename[j] = '\0';
        fd = open(filename, O_RDONLY);
        if (fd == -1)
        {
            perror("open");
            exit(1);
        }
        while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
            write(1, buffer, bytes_read);
        if (bytes_read == -1)
        {
            perror("read");
            close(fd);
            exit(1);
        }

        close(fd);
        exit(0);
    }
    else
        waitpid(pid, NULL, 0);
    printf("\n");
}

void execute_rm(char *inpt)
{
    int i = 2;
    pid_t pid;
    char filename[70000];
    int j = 0;

    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    if (inpt[i] == '\0')
    {
        write(1, "error: rm needs a filename\n", 27);
        return;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t' && j < 255)
            filename[j++] = inpt[i++];
        filename[j] = '\0';

        if (access(filename, F_OK) == 0)
        {
            if (unlink(filename) == -1)
            {
                perror("rm");
                exit(1);
            }
        }
        else
        {
            perror("rm");
            exit(1);
        }
        exit(0);
    }
    else
        waitpid(pid, NULL, 0);
}

void execute_touch(char *inpt)
{
    int i = 5;
    pid_t pid;
    char filename[70000];
    int j = 0;

    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    if (inpt[i] == '\0')
    {
        write(1, "error: touch needs a filename\n", 30);
        return;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t' && j < 255)
            filename[j++] = inpt[i++];
        filename[j] = '\0';
        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
        {
            perror("touch");
            exit(1);
        }
        close(fd);
        exit(0);
    }
    else
        waitpid(pid, NULL, 0);
}

void execute_exit(char *inpt)
{
    int i = 4;
    int sign = 1;
    int status = 0;

    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    if ((inpt[i] == '-' && inpt[i+1] == '-') || (inpt[i] == '+' && inpt[i+1] == '+'))
    {
        write(1, "error: numeric argument required\n", 34);
        return;
    }
    if (inpt[i] == '-' && inpt[i+1] != '-')
    {
        sign *= -1;
        i++;
    }
    if (inpt[i] != '\0')
    {
        status = 0;
        while (inpt[i])
        {
            if (inpt[i] >= '0' && inpt[i] <= '9')
            {
                status = status * 10 + (inpt[i] - '0');
                i++;
            }
            else
            {
                write(1, "error: numeric argument required\n", 34);
                return;
            }
        }
    }
    write(1, "exit\n", 5);
    exit(status * sign);
}

//void  execute_cd(char *inpt)

//void  execute_unset(char *inpt)

//void  execute_env(char *inpt)

//void  execute_export(char *inpt)

void    execute_cd(char *inpt)
{
    /*Relative Path: A path that is relative to the current directory. For example, cd folder changes to a subdirectory folder within the current directory.
    Absolute Path: A path that starts from the root directory. For example, cd /home/user/folder changes to the specified directory starting from the root.*/

    //init necessary var
    int     i;//start index after c comm
    char    *path;//buffer to hold direct path
    int     j;//index in the path buffer

    i = 2;
    j = 0;
    //skip whitespace
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    //extract dir path
    path = malloc(sizeof(char *) * ft_strlen(inpt));
    while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t' && j < 255)
        path[j++] = inpt[i++];
    path[j] = '\0';
    //Edge case handling, eg. no dir path provided
    if (j == 0)
    {
        ft_printf("error: cd needs a directory path\n");
        return;
    }
    // Change directory
    if (chdir(path) != 0)
    {
        perror("cd");
    }
}

extern char **environ;

void execute_unset(char *inpt)
{
    int i = 6; // Start after the "unset" command
    char varname[256];
    long unsigned int j = 0;

    // Skip leading spaces and tabs
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;

    // Check if there is a variable name provided
    if (inpt[i] == '\0')
    {
        ft_printf("error: unset needs a variable name\n");
        return;
    }

    // Extract the variable name
    while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t' && j < sizeof(varname) - 1)
        varname[j++] = inpt[i++];
    varname[j] = '\0';

    // Remove the variable from the environment
    j = 0;
    while (environ[j] != NULL)
    {
        if (strncmp(environ[j], varname, strlen(varname)) == 0 && environ[j][strlen(varname)] == '=')
        {
            // Shift all subsequent environment variables one position to the left
            while (environ[j + 1] != NULL)
            {
                environ[j] = environ[j + 1];
                j++;
            }
            environ[j] = NULL;
            break;
        }
        j++;
    }
}

void    execute_env(void)
{
    /*function to print the environment variables*/
    // Initialization
    int i;
    pid_t pid;
    // Fork a new process
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        // Child process to print the environment variables
        i = 0;
        while (environ[i] != NULL)
        {
            ft_printf("%s\n", environ[i]);
            i++;
        }
        exit(0);
    }
    else
    {
        // Wait for the child process to finish
        if (waitpid(pid, NULL, 0) == -1)
        {
            perror("waitpid");
        }
    }
}

void    execute_export(char *inpt)
{
    int                 i;
    char                varname[256];
    char                value[256];
    long unsigned int   j;
    int                 env_size;
    char                **new_environ;
    int                 k;//to copy the environ array into new_environ

    i = 6;
    j = 0;
    env_size = 0;
    k = 0;
    ///whitespace
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    //input after command
    if (inpt[i] == '\0')
    {
        ft_printf("error: export missing operand\n");
        return;
    }
    //extracting the variable name UNTIL = found
    while (inpt[i] && inpt[i] != '=' && inpt[i] != '\t' && j < sizeof(varname) - 1)
        varname[j++] = inpt[i++];
    varname[j] = '\0';
    //ENSURES THAT '=' character is actually present after the variable name
    if (inpt[i] != '=')
    {
        ft_printf("error: export missing '=' after variable name\n");
        return;
    }
    i++;
    //now we extract the value name after =
    j = 0;
    while (inpt[i] && j < sizeof(value) - 1)
        value[j++] = inpt[i++];
    value[j] = '\0';
    //from here we need to work on the setenv replacement
    // Get the size of the environment variables
    while (environ[env_size] != NULL)
        env_size++;
    //allocate memory for new env var
    new_environ = malloc(sizeof(char *) * (env_size + 2));
    if (!new_environ)
    {
        perror("malloc");
        return;
    }
    //copy the existing environment variables to new_environ
    while(k < env_size)
    {
        new_environ[k] = environ[k];
        k++;
    }
    // Allocate memory and set the new variable
    new_environ[env_size] = malloc(ft_strlen(varname) + ft_strlen(value) + 2);//+2 for = and \0
    if (!new_environ[env_size])
    {
        perror("malloc");
        free(new_environ);
        return;
    }
    //concatenate the variable name and value into new variable(replaces sprintf)
    j = 0;
    while (varname[j] != '\0')
    {
        new_environ[env_size][j] = varname[j];
        j++;
    }
    new_environ[env_size][j++] = '=';
    i = 0;
    while (value[i] != '\0')
    {
        new_environ[env_size][j++] = value[i++];
    }
    new_environ[env_size][j] = '\0';
    // Set the last element of the new_environ array to NULL
    new_environ[env_size + 1] = NULL;
    // Update the environment variables
    environ = new_environ;
    //do we need to free the old environ?
}
