/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:28:58 by ego               #+#    #+#             */
/*   Updated: 2025/02/11 20:24:51 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Close all file descriptors in data.
 * 
 * @param data Pointer to the data structure.
 */
void	close_fds(t_data *data)
{
	int	i;

	if (data->fd_in > -1)
		close(data->fd_in);
	if (data->fd_out > -1)
		close(data->fd_out);
	i = -1;
	while (++i < 2 * (data->children - 1))
		close(data->pipe[i]);
}

/**
 * @brief Entirely frees an array of strings.
 * 
 * @param s The array to be freed.
 * 
 * @return NULL.
 */
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

/**
 * @brief Entirely frees an array of arrays of strings.
 * 
 * @param s The array to be freed.
 */
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

/**
 * @brief Frees everything there is to free in
 * a data structure, if allocated. Also closes
 * all opened file descriptors.
 * 
 * @param data Pointer to the data structure.
 */
void	free_data(t_data *data)
{
	close_fds(data);
	if (data->cmds)
		free_cmds(data->cmds);
	if (data->pids)
		free(data->pids);
	if (data->pipe)
		free(data->pipe);
	if (data->found)
		free(data->found);
	if (data->fd_in > -1)
		close(data->fd_in);
	if (data->fd_out > -1)
		close(data->fd_out);
	if (data->fd_stdin > -1)
		close(data->fd_stdin);
	return ;
}

/**
 * @brief Prints two error messages and optionnally a newline.
 * Exits the program cleanly afterwards with code 1.
 * 
 * @param data Pointer to the data structure.
 * @param msg1 First message to be printed.
 * @param msg2 Second printed to be printed.
 * @param nl 1 if newline wanted, 0 otherwise.
 */
void	exit_error(t_data *data, char *msg1, char *msg2, int nl)
{
	if (msg1)
		ft_putstr_fd(msg1, STDERR_FILENO);
	if (msg2)
		ft_putstr_fd(msg2, STDERR_FILENO);
	if (nl)
		ft_putchar_fd('\n', STDERR_FILENO);
	free_data(data);
	exit(1);
	return ;
}
