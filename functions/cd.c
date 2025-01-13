/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:36:11 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/13 10:55:36 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*void    execute_cd(char *inpt)
{
    int     i;
    char    *path;
    int     j;

    i = 2;
    j = 0;
    g_exit_status = 0;
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    if (i == 2 && inpt[i++] != '\0')
    {
        printf("error: need a space between function and argument\n");
        g_exit_status = 1;
        return;
    }
    path = malloc(sizeof(char *) * ft_strlen(inpt));
    while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t' && j < 255)
        path[j++] = inpt[i++];
    path[j] = '\0';
    if (j == 0 || path == NULL)
    {
        ft_printf("error: cd needs a directory path\n");
        g_exit_status = 1;
        return;
    }
    if (chdir(path) != 0)
    {
        perror("cd");
        g_exit_status = 1;
    }
}*/

void	handle_cd_error(int error_type)
{
	if (error_type == 1)
		printf("error: need a space between function and argument\n");
	else if (error_type == 2)
		ft_printf("error: cd needs a directory path\n");
	g_exit_status = 1;
}

char	*extract_path(char *inpt, int *index)
{
	char	*path;
	int		i;
	int		j;

	i = *index;
	j = 0;
	path = malloc(sizeof(char) * 256);
	if (!path)
		return (NULL);
	while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t' && j < 255)
		path[j++] = inpt[i++];
	path[j] = '\0';
	*index = i;
	return (path);
}

void	execute_cd(char *inpt)
{
	int		i;
	char	*path;

	i = 2;
	g_exit_status = 0;
	while (inpt[i] == ' ' || inpt[i] == '\t')
		i++;
	if (i == 2 && inpt[i++] != '\0')
	{
		handle_cd_error(1);
		return ;
	}
	path = extract_path(inpt, &i);
	if (!path || path[0] == '\0')
	{
		free(path);
		handle_cd_error(2);
		return ;
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		g_exit_status = 1;
	}
	free(path);
}
