/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:13:19 by ego               #+#    #+#             */
/*   Updated: 2025/02/11 20:07:24 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Counts the number of "words" delimited by the given
 * character in the given string.
 * 
 * @param s String to parse.
 * @param c Delimiter.
 * 
 * @return Number of words.
 */
static size_t	ft_split_count_words(const char *s, char c)
{
	size_t	i;
	size_t	nb_words;
	int		is_word;

	i = 0;
	nb_words = 0;
	is_word = 0;
	while (s[i])
	{
		if (s[i] != c && is_word == 0)
		{
			is_word = 1;
			nb_words++;
		}
		if (s[i] == c && is_word == 1)
			is_word = 0;
		i++;
	}
	return (nb_words);
}

/**
 * @brief Duplicates a given string up to n first characters.
 * 
 * @param s String to duplicate.
 * @param n Number of characters wanted.
 * 
 * @return Duplicated string. NULL if allocation failed or n is zero.
 */
static char	*ft_strndup(const char *s, size_t n)
{
	char	*ndup;
	size_t	i;

	if (!n)
		return (NULL);
	ndup = (char *)malloc((n + 1) * sizeof(char));
	if (!ndup)
		return (NULL);
	i = 0;
	while (s[i] && i < n)
	{
		ndup[i] = s[i];
		i++;
	}
	ndup[i] = '\0';
	return (ndup);
}

/**
 * @brief Gets the next word length.
 * 
 * @param s String to parse. Pointer should start at a word.
 * @param c Delimiter.
 * 
 * @return Word length.
 */
static size_t	ft_split_get_word_len(const char *s, char c)
{
	size_t	word_len;

	word_len = 0;
	while (*s && *s != c)
	{
		word_len++;
		s++;
	}
	return (word_len);
}

/**
 * @brief Allocates and returns an array of strings obtained by
 * splitting the string s using the character c as a delimiter.
 * The array ends with a NULL pointer.
 * 
 * @param s The string to be split.
 * @param c The delimiter character.
 * 
 * @return The array of new strings resulting from the split.
 * NULL if the allocation fails.
 */
char	**ft_split(const char *s, char c)
{
	char	**split;
	size_t	i;
	size_t	word_len;

	if (!s)
		return (NULL);
	split = (char **)malloc((ft_split_count_words(s, c) + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	i = 0;
	while (*s)
	{
		if (*s && *s == c)
			s++;
		if (*s && *s != c)
		{
			word_len = ft_split_get_word_len(s, c);
			split[i] = ft_strndup(s, word_len);
			if (!(split[i++]))
				return (free_split(split));
			s += word_len;
		}
	}
	split[i] = NULL;
	return (split);
}
