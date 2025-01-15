/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:08:49 by tjehaes           #+#    #+#             */
/*   Updated: 2024/02/26 17:18:40 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	lilen;

	i = 0;
	j = 0;
	lilen = ft_strlen(little);
	if (lilen == 0 || big == little)
		return ((char *)big);
	while (i < len && big[i] != '\0')
	{
		while (i + j < len && big[i + j] == little[j] && little[j] != '\0')
			j++;
		if (j == lilen)
			return ((char *)(big + i));
		i++;
		j = 0;
	}
	return (0);
}

char	*ft_strstr(const char *big, const char *little)
{
	size_t i;
	size_t j;
	size_t little_len;

	i = 0;
	if (!*little)
		return ((char *)big);
	little_len = ft_strlen(little);
	while (big[i] != '\0')
	{
		j = 0;
		while (big[i + j] == little[j] && little[j] != '\0')
			j++;
		if (j == little_len)
			return ((char *)(big + i));
		i++;
	}
	return (NULL);
}