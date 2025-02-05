/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:28:58 by ego               #+#    #+#             */
/*   Updated: 2025/02/05 20:05:21 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*	ft_free
*	Frees a char pointer only if allocated,
*	and set it to NULL afterwards.
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

/*	free_split
*	Entirely frees a double char pointer.
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

/*	free_cmds
*	Entirely frees a triple char pointer.
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

/*	free_data
*	Frees everything there is to free in
*	a data structure, if allocated.
*/
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

/*	exit_error
*	Prints two error messages, a newline 
*	and exits the program cleanly.
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
