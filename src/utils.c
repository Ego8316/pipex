/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:28:58 by ego               #+#    #+#             */
/*   Updated: 2025/02/05 19:36:50 by ego              ###   ########.fr       */
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

char	**free_split(char **s)
{
	int	i;

	i = 0;
	while (s && s[i])
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
	while (s && s[i])
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
	if (data->errors)
		free(data->errors);
	if (data->pids)
		free(data->pids);
	if (data->pipe)
		free(data->pipe);
	if (data->fd_in >= 0)
		close(data->fd_in);
	if (data->fd_out >= 0)
		close(data->fd_out);
	if (data->fd_stdin >= 0)
		close(data->fd_stdin);
	if (data->here_doc == 1)
		unlink(TMP);
	return ;
}

void	exit_error(t_data *data, char *msg1, char *msg2, int nl)
{
	if (msg1)
		ft_putstr_fd(msg1, 2);
	if (msg2)
		ft_putstr_fd(msg2, 2);
	if (nl)
		ft_putchar_fd('\n', 2);
	free_data(data);
	exit(1);
	return ;
}
