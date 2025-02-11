/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 21:50:10 by ego               #+#    #+#             */
/*   Updated: 2025/02/11 20:25:19 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Frees the pointed string only if allocated
 * and set it to NULL afterwards.
 * 
 * @param s Pointer to a string.
 * 
 * @return 1.
 */
int	ft_free(char **s)
{
	if (s && *s)
	{
		free(*s);
		*s = NULL;
	}
	return (1);
}

/**
 * @brief Duplicates a given string up to n first characters.
 * 
 * @param s String to duplicate.
 * @param n Number of characters wanted.
 * @param error Set to 1 if the allocation fails.
 * 
 * @return Duplicated string. NULL if the allocation fails or n is zero.
 */
static char	*ft_strndup(const char *s, size_t n, int *error)
{
	char	*ndup;
	size_t	i;

	if (!s || !n)
		return (ft_strdup("", error));
	ndup = (char *)ft_calloc(n + 1, sizeof(char), error);
	if (!ndup)
		return (NULL);
	i = 0;
	while (s[i] && i < n)
	{
		ndup[i] = s[i];
		i++;
	}
	return (ndup);
}

/**
 * @brief Reads from given file descriptor and appends content
 * to the stash until finding a newline.
 * 
 * @param fd File descriptor.
 * @param stash Pointer to the stash.
 * @param error Set to 1 if an allocation fails.
 * 
 * @return 0 if an error occurs, 1 otherwise.
 */
static int	ft_get_to_next_nl(int fd, char **stash, int *error)
{
	char		*buf;
	char		*temp;
	ssize_t		r;

	buf = (char *)ft_calloc(BUFFER_SIZE + 1, sizeof(char), error);
	if (!buf)
		return (0);
	r = 1;
	while (r > 0)
	{
		r = read(fd, buf, BUFFER_SIZE);
		if (r == -1)
			return (1 - ft_free(&buf));
		buf[r] = '\0';
		temp = ft_strjoin(*stash, buf, error);
		ft_free(stash);
		*stash = temp;
		if (!*stash)
			return (1 - ft_free(&buf));
		if (ft_char_in_str('\n', *stash))
			return (ft_free(&buf));
	}
	return (ft_free(&buf));
}

/**
 * @brief Parses the stash to give the part before newline
 * and keep in the stash only what is after.
 * 
 * @param stash Pointer to the stash.
 * @param error Set to 1 if an allocation fails.
 * 
 * @return Allocated string containing what is before newline.
 * NULL if an error occured or if the content is empty.
 */
static char	*ft_split_content(char **stash, int *error)
{
	char	*temp;
	char	*line;
	size_t	i;

	temp = ft_strdup(*stash, error);
	if (!temp)
		return (NULL);
	ft_free(stash);
	i = 0;
	while (temp[i] && temp[i] != '\n')
		i++;
	if (temp[i] == '\n')
		i++;
	line = ft_strndup(temp, i, error);
	if (line)
		*stash = ft_strdup(temp + i, error);
	ft_free(&temp);
	if (!line || !*stash)
	{
		ft_free(&line);
		ft_free(stash);
		return (NULL);
	}
	return (line);
}

/**
 * @brief Gives the next line in an opened file descriptor.
 * Can be called with NULL pointer to free the stash.
 * 
 * @param fd File descriptor.
 * @param fd Set to 1 if an allocation fails.
 * 
 * @return The next line. NULL if end of file has been
 * reached or if an error occured.
 */
char	*get_next_line(int fd, int *error)
{
	static char	*stash[FD_MAX];
	char		*line;

	if (fd < 0 || fd > FD_MAX || read(fd, 0, 0) == -1)
	{
		ft_free(&stash[fd]);
		return (NULL);
	}
	line = NULL;
	if (error && ft_get_to_next_nl(fd, &stash[fd], error) == 0)
	{
		ft_free(&stash[fd]);
		return (NULL);
	}
	if (error && stash[fd] && *stash[fd])
		line = ft_split_content(&stash[fd], error);
	if (!line || !(*line) || *error || !error)
	{
		ft_free(&stash[fd]);
		ft_free(&line);
		return (NULL);
	}
	return (line);
}
