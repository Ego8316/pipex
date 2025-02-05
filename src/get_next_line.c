/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 21:50:10 by ego               #+#    #+#             */
/*   Updated: 2025/02/04 19:47:27 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_free(char **s)
{
	if (s && *s)
	{
		free(*s);
		*s = NULL;
	}
	return (1);
}

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
	if (ft_get_to_next_nl(fd, &stash[fd], error) == 0)
	{
		ft_free(&stash[fd]);
		return (NULL);
	}
	if (stash[fd] && *stash[fd])
		line = ft_split_content(&stash[fd], error);
	if (!line || !(*line))
	{
		ft_free(&stash[fd]);
		ft_free(&line);
		return (NULL);
	}
	return (line);
}
