/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_setenv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:17:14 by mdaman            #+#    #+#             */
/*   Updated: 2025/01/22 15:29:53 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	copy_name_value(char *new_var, const char *name, const char *value,
		int name_len)
{
	ft_memcpy(new_var, name, name_len);
	new_var[name_len] = '=';
	ft_memcpy(new_var + name_len + 1, value, ft_strlen(value));
	new_var[name_len + ft_strlen(value) + 1] = '\0';
}

int	check_new_var_allocation(char *new_var)
{
	if (!new_var)
	{
		perror("malloc");
		return (1);
	}
	return (0);
}

char	*create_new_var(const char *name, const char *value)
{
	char	*new_var;
	int		name_len;
	int		value_len;

	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	new_var = malloc(name_len + value_len + 2);
	if (!new_var)
	{
		perror("malloc");
		return (NULL);
	}
	ft_memcpy(new_var, name, name_len);
	new_var[name_len] = '=';
	ft_memcpy(new_var + name_len + 1, value, value_len);
	new_var[name_len + value_len + 1] = '\0';
	return (new_var);
}

int	update_existing_var(t_env *env, int index, const char *args[],
		int overwrite)
{
	char	*new_var;

	if (!overwrite)
		return (0);
	free(env->variables[index]);
	new_var = create_new_var(args[0], args[1]);
	if (!new_var)
		return (-1);
	env->variables[index] = new_var;
	return (0);
}

int	custom_setenv(t_env *env, const char *name, const char *value,
		int overwrite)
{
	int			i;
	int			name_len;
	char		*new_var;
	const char	*args[2];

	args[0] = name;
	args[1] = value;
	name_len = ft_strlen(name);
	i = 0;
	while (i < env->size)
	{
		if (ft_strncmp(env->variables[i], name, name_len) == 0
			&& env->variables[i][name_len] == '=')
			return (update_existing_var(env, i, (const char **)args,
					overwrite));
		i++;
	}
	new_var = create_new_var(args[0], args[1]);
	if (check_new_var_allocation(new_var) == 1)
		return (-1);
	copy_name_value(new_var, args[0], args[1], name_len);
	env->variables[env->size] = new_var;
	env->variables[++env->size] = NULL;
	return (0);
}
