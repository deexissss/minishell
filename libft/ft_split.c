/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdaman <mdaman@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:25:56 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 13:32:31 by mdaman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	count(const char *str, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (*str)
	{
		if (*str != c && i == 0)
		{
			i = 1;
			count++;
		}
		else if (*str == c)
			i = 0;
		str++;
	}
	return (count);
}

char	*new_strdup(const char *str, int start, int finish)
{
	char	*new_word;
	int		i;

	i = 0;
	new_word = malloc(sizeof(char) * ((finish - start) + 1));
	while (start < finish)
	{
		new_word[i] = str[start];
		i++;
		start++;
	}
	new_word[i] = '\0';
	return (new_word);
}

void	free_split(char **split, size_t j)
{
	size_t	i;

	i = 0;
	while (i < j)
		free(split[i++]);
	free(split);
}

void	fill_split(char **split, char const *s, char c, size_t i)
{
	size_t	j;
	int		index;

	j = 0;
	index = -1;
	while (i <= ft_strlen(s))
	{
		if (s[i] != c && index < 0)
			index = i;
		else if ((s[i] == c || i == ft_strlen(s)) && index >= 0)
		{
			split[j] = new_strdup(s, index, i);
			if (!split[j])
			{
				free_split(split, j);
				return ;
			}
			j++;
			index = -1;
		}
		i++;
	}
	split[j] = 0;
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	split = malloc(sizeof(char *) * (count(s, c) + 1));
	if (!split)
		return (0);
	fill_split(split, s, c, i);
	return (split);
}
