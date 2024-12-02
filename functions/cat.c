#include "../minishell.h"
/*
void execute_cat(char *inpt)
{
    int     i;
    int     fd;
    char    buffer[42];
    char    filename[70000];
    int     bytes_read;
    pid_t   pid;
    int     j;

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
}*/
void print_error_and_exit(const char *message)
{
    perror(message);
    exit(1);
}

void read_and_print_file(const char *filename)
{
    int fd;
    char buffer[42];
    int bytes_read;

    fd = open(filename, O_RDONLY);
    if (fd == -1)
        print_error_and_exit("open");

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
        write(1, buffer, bytes_read);

    if (bytes_read == -1)
        print_error_and_exit("read");

    close(fd);
}

void handle_child_process(char *inpt, int i)
{
    char filename[70000];
    int j = 0;

    while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t' && j < 70000)
        filename[j++] = inpt[i++];
    filename[j] = '\0';

    read_and_print_file(filename);
    exit(0);
}

void execute_cat(char *inpt)
{
    int i = 3;
    pid_t pid;

    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    if (inpt[i] == '\0')
    {
        printf("error : cat needs a file name\n");
        return;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
        handle_child_process(inpt, i);
    else
        waitpid(pid, NULL, 0);
    printf("\n");
}
