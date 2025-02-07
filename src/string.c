/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:09:41 by ego               #+#    #+#             */
/*   Updated: 2025/02/07 13:27:47 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
