/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:07:21 by ego               #+#    #+#             */
/*   Updated: 2025/02/11 19:59:13 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Redirects the standard input to the provided input file
 * descriptor and standard output to the provided output file descriptor
 * using dup2. If dup2 fails, exits the program cleanly.
 * 
 * @param data Pointer to the data structure.
 * @param fd_in Input file descriptor to be used as new STDIN.
 * @param fd_out Output file descriptor to be used as new STDOUT.
 */
static void	redirect_io(t_data *data, int fd_in, int fd_out)
{
	if (dup2(fd_in, STDIN_FILENO) == -1)
		exit_error(data, "dup2: ", strerror(errno), 1);
	if (dup2(fd_out, STDOUT_FILENO) == -1)
		exit_error(data, "dup2: ", strerror(errno), 1);
	return ;
}

/**
 * @brief If the corresponding command is a valid command,
 * redirects standard input and output, closes unused file
 * descriptors and starts exceve. If the command is invalid
 * or if it is the first command but the input file is invalid,
 * exits the child process and prints an error message like bash.
 * 
 * @param data Pointer to the data structure.
 * @param i Child/command index.
 */
static void	child_routine(t_data *data, int i)
{
	if (data->found[i] && !(i == 0 && data->fd_in == -1))
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
		if (!(i == 0 && data->fd_in == -1))
		{
			ft_putstr_fd(data->cmds[i][0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
		}
		free_data(data);
		exit(127);
	}
	return ;
}

/**
 * @brief Waits for each child routine to end.
 * 
 * @param data Pointer to the data structure.
 * 
 * @return Exit code of the last child.
 */
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

/**
 * Forks the program for each command and calls
 * the child routine for each child  created.
 * 
 * @param data Pointer to the data structure.
 * 
 * @return Exit code of the last command.
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

/**
 * Ensures the argument given is formatted correctly.
 * Prints a help message if required. Otherwise
 * initializes data and calls pipex.
 */
int	main(int ac, char **av, char **envp)
{
	t_data	data;
	int		i;
	int		exit_code;

	if (ac < 5 || (ac >= 2 && ac < 6 && !ft_strncmp("here_doc", av[1], 9)))
		return (put_help_message());
	i = 0;
	while (i < ac)
	{
		if (!av[i][0])
			return (put_help_message());
		i++;
	}
	data = data_init(ac, av, envp);
	exit_code = pipex(&data);
	if (data.here_doc == 1)
		unlink(TMP);
	free_data(&data);
	return (exit_code);
}
