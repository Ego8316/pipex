/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:07:21 by ego               #+#    #+#             */
/*   Updated: 2025/02/06 20:24:02 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	redirect_io(t_data *data, int fd_in, int fd_out)
{
	if (dup2(fd_in, STDIN_FILENO) == -1)
		exit_error(data, "dup2: ", strerror(errno), 1);
	if (dup2(fd_out, STDOUT_FILENO) == -1)
		exit_error(data, "dup2: ", strerror(errno), 1);
	return ;
}

static void	child_routine(t_data *data, int i)
{
	if (data->found[i])
	{
		if (i == 0)
			redirect_io(data, data->fd_in, data->pipe[1]);
		else if (i == data->children - 1)
			redirect_io(data, data->pipe[2 * i - 2], data->fd_out);
		else
			redirect_io(data, data->pipe[2 * i - 2], data->pipe[2 * i + 1]);
		close_fds(data);
		if (execve(data->cmds[i][0], data->cmds[i], data->envp) == -1)
			exit_error(data, "execve: ", strerror(errno), 1);
	}
	else
	{
		free_data(data);
		exit(127);
	}
	return ;
}

static int	parent_routine(t_data *data)
{
	int		i;
	int		exit_code;
	int		status;
	pid_t	wpid;

	close_fds(data);
	i = 0;
	exit_code = 0;
	while (i < data->children)
	{
		wpid = waitpid(data->pids[i], &status, 0);
		if (wpid == data->pids[i] && WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		i++;
	}
	return (exit_code);
}

/*	pipex
*	
*/
int	pipex(t_data *data)
{
	int	i;
	int	exit_code;

	i = 0;
	while (i < data->children)
	{
		data->pids[i] = fork();
		if (data->pids[i] == -1)
			exit_error(data, "fork: ", strerror(errno), 1);
		else if (data->pids[i] == 0)
			child_routine(data, i);
		i++;
	}
	exit_code = parent_routine(data);
	return (exit_code);
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;
	int		exit_code;

	if (ac < 5 || (ac >= 2 && ac < 6 && !ft_strncmp("here_doc", av[1], 9)))
		return (put_help_message());
	data = data_init(ac, av, envp);
	exit_code = pipex(&data);
	free_data(&data);
	return (exit_code);
}
