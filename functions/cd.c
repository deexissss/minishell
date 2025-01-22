/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:36:11 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 10:15:55 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_cd_error(int error_type)
{
	if (error_type == 1)
		printf("error: need a space between function and argument\n");
	else if (error_type == 2)
		printf("error: cd needs a directory path\n");
	g_env.exit_status = 1;
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
	g_env.exit_status = 0;
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
		g_env.exit_status = 1;
	}
	free(path);
}
