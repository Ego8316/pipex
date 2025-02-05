/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 20:13:29 by ego               #+#    #+#             */
/*   Updated: 2025/02/05 14:38:21 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_char_in_str(char c, char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (1);
		i++;
	}
	return (0);
}

void	*ft_calloc(size_t nmemb, size_t size, int *error)
{
	void	*array;
	size_t	i;

	array = (void *)malloc(nmemb * size);
	if (!array)
	{
		*error = 1;
		return (NULL);
	}
	i = 0;
	while (i < nmemb * size)
	{
		((unsigned char *)array)[i] = '\0';
		i++;
	}
	return (array);
}

int	ft_strlen(const char *s)
{
	int	len;

	len = 0;
	while (s && s[len])
		len++;
	return (len);
}
