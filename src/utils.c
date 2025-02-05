/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:28:58 by ego               #+#    #+#             */
/*   Updated: 2025/02/05 01:55:16 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	free_strs(char *s1, char *s2, char *s3)
{
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	if (s3)
		free(s3);
	return (0);
}

char	**free_split(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	if (s)
		free(s);
	return (NULL);
}

void	free_cmds(char ***s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free_split(s[i]);
		i++;
	}
	if (s)
		free(s);
	return ;
}

void	free_data(t_data *data)
{
	if (data->cmds)
		free_cmds(data->cmds);
	if (data->pids)
		free(data->pids);
	if (data->pipe)
		free(data->pipe);
	return ;
}

void	exit_error(t_data *data, const char *msg1, const char *msg2)
{
	free_data(data);
	if (msg1)
		ft_putstr_error(msg1);
	if (msg2)
		ft_putstr_error(msg2);
	exit(1);
	return ;
}
