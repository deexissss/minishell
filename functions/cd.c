/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:36:11 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 15:07:31 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_cd_error(t_env *env, int error_type)
{
	if (error_type == 1)
		printf("error: need a space between function and argument\n");
	else if (error_type == 2)
		printf("error: cd needs a directory path\n");
	env->exit_status = 1;
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

void	update_pwd(t_env *env)
{
	int		i;
	char	path[4096];

	i = 0;
	if (getcwd(path, sizeof(path)) != NULL)
	{
		while (i < env->size)
		{
			if (ft_strncmp(env->variables[i], "PWD=", 4) == 0)
			{
				free(env->variables[i]);
				env->variables[i] = ft_strjoin("PWD=", path);
				return ;
			}
			i++;
		}
		env->variables[env->size] = ft_strjoin("PWD=", path);
		env->size++;
	}
	else
	{
		perror("getcwd");
		env->exit_status = 1;
	}
}

int	check_path2(t_env *env, char *path)
{
	if (!path || path[0] == '\0')
	{
		free(path);
		handle_cd_error(env, 2);
		return (1);
	}
	return (0);
}

void	execute_cd(t_env *env, char *inpt)
{
	int		i;
	char	*path;

	i = 2;
	env->exit_status = 0;
	while (inpt[i] == ' ' || inpt[i] == '\t')
		i++;
	if (i == 2 && inpt[i++] != '\0')
	{
		handle_cd_error(env, 1);
		return ;
	}
	path = extract_path(inpt, &i);
	if (check_path2(env, path) == 1)
		return ;
	if (chdir(path) != 0)
	{
		perror("cd");
		env->exit_status = 1;
	}
	else
		update_pwd(env);
	free(path);
}
