/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 20:13:29 by ego               #+#    #+#             */
/*   Updated: 2025/02/11 20:12:46 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Iterates through the string s to see
 * if the character c can be found inside.
 * 
 * @param c Character to be found.
 * @param s String to search.
 * 
 * @return 1 if the caracter has been found,
 * 0 otherwise.
 */
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

/**
 * @brief Allocates memory for an array of nmemb elements of
 * size bytes each and returns a pointer to the allocated memory.
 * The memory is set to zero.
 * 
 * @param nmemb Number of elements.
 * @param size Size of each element type.
 * @param error Set to 1 if the allocation fails.
 * 
 * @return The allocated array or NULL if the allocation failed.
 */
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

/**
 * @brief Computes the length of the given string.
 * 
 * @param s String whose length is to be computed.
 * 
 * @return The length of the string. 0 if the string
 * is empty.
 */
int	ft_strlen(const char *s)
{
	int	len;

	len = 0;
	while (s && s[len])
		len++;
	return (len);
}
