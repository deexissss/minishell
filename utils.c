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

void execute_ls() {
    DIR *dir;
    struct dirent *entry;
    pid_t pid;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    else if (pid == 0) {
        dir = opendir(".");
        if (dir == NULL) {
            perror("opendir");
            exit(1);
        }

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.') {
                write(1, entry->d_name, ft_strlen(entry->d_name));
                write(1, "  ", 3);
            }
        }
        write(1, "\n", 1);
        
        closedir(dir);
        exit(0);
    }
    else {
        waitpid(pid, NULL, 0);
    }
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

    i = 4;
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    while (inpt[i])
    {
        write(1, &inpt[i], 1);
        i++;
    }
}

void    execute_sleep(char *inpt)
{
    int     i;
    int     time;
    pid_t pid = fork();

    i = 5;
    time = 0;
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    if (inpt[i] == '\0')
        ft_printf("error : sleep need a time");
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
}