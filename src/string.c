/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:09:41 by ego               #+#    #+#             */
/*   Updated: 2025/02/11 20:20:48 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Compares the first n bytes of the two strings s1 and s2.
 * 
 * @param s1 Pointer to the first string.
 * @param s2 Pointer to the second string.
 * @param n The number of bytes to compare.
 * 
 * @return - 0 if the first n bytes of both strings are equal,
 * a signed value otherwise.
 */
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int		diff;
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		diff = (unsigned char)s1[i] - (unsigned char)s2[i];
		if (diff)
			return (diff);
		i++;
	}
	return (0);
}

/**
 * @brief Duplicates a given string.
 * 
 * @param s String to duplicate.
 * @param error Set to 1 if the allocation fails.
 * 
 * @return Duplicated string. NULL if the allocation fails.
 */
char	*ft_strdup(const char *s, int *error)
{
	char	*dup;
	size_t	i;

	if (!s)
		return (ft_strdup("", error));
	i = 0;
	while (s[i])
		i++;
	dup = (char *)ft_calloc(i + 1, sizeof(char), error);
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	return (dup);
}

/**
 * @brief Allocates and returns a new string, which is the result
 * of the concatenation of s1 and s2. If the first one is empty,
 * only duplicates the second one.
 * 
 * @param s1 The prefix string.
 * @param s2 The suffix string.
 * @param error Set to 1 if the allocation fails.
 * 
 * @return The new string. NULL if the allocation fails.
 */
char	*ft_strjoin(const char *s1, const char *s2, int *error)
{
	size_t	i;
	size_t	j;
	char	*join;

	if (!s1)
		return (ft_strdup(s2, error));
	j = 0;
	while (s1[j])
		j++;
	i = 0;
	while (s2[i])
		i++;
	join = (char *)ft_calloc((i + j + 1), sizeof(char), error);
	if (!join)
		return (NULL);
	i = -1;
	while (s1[++i])
		join[i] = s1[i];
	j = -1;
	while (s2[++j])
		join[i + j] = s2[j];
	return (join);
}

/**
 * @brief Allocates and returns a new string, which is the
 * result of the concatenation of s1, s2 and s3.
 * 
 * @param s1 First string.
 * @param s2 Second string.
 * @param s3 Third string.
 * 
 * @return The new string. NULL if the allocation fails.
 */
char	*join_strs(char *s1, char *s2, char *s3)
{
	char	*join;
	int		len1;
	int		len2;
	int		len3;
	int		i;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	join = (char *)malloc((len1 + len2 + len3 + 1) * sizeof(char));
	if (!join)
		return (NULL);
	i = -1;
	while (++i < len1 && s1)
		join[i] = s1[i];
	i = -1;
	while (++i < len2 && s2)
		join[i + len1] = s2[i];
	i = -1;
	while (++i < len3 && s3)
		join[i + len1 + len2] = s3[i];
	join[i + len1 + len2] = 0;
	return (join);
}
