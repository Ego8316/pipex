/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcavet <hcavet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:48:40 by ego               #+#    #+#             */
/*   Updated: 2025/02/27 12:16:40 by hcavet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Initializes an empty data structure.
 * 
 * @return The new data structure.
 */
static t_data	data_new(void)
{
	t_data	data;

	data.cmds = NULL;
	data.envp = NULL;
	data.pids = NULL;
	data.pipe = NULL;
	data.found = NULL;
	data.children = 0;
	data.here_doc = 0;
	data.fd_in = -1;
	data.fd_out = -1;
	data.fd_stdin = -1;
	return (data);
}

/**
 * @brief Duplicates standard input, reads from it and writes in the
 * .tmp temporary file until limiter is found. If EOF is reached
 * before finding limiter, prints bash warning. get_next_line
 * is called afterwards with NULL pointer to empty the stash.
 */
void	read_here_doc(t_data *data, char *limiter)
{
	int		error;
	int		eof;
	char	*line;

	error = 0;
	eof = 0;
	data->fd_stdin = dup(STDIN_FILENO);
	if (data->fd_stdin == -1)
		exit_error(data, "dup: ", strerror(errno), 1);
	while (!eof)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(data->fd_stdin, &error);
		if (error)
			exit_error(data, "malloc: ", strerror(errno), 1);
		if (!line)
			eof = put_bash_warning(limiter);
		else if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == '\n')
			eof = 1;
		else
			ft_putstr_fd(line, data->fd_in);
		ft_free(&line);
	}
	get_next_line(data->fd_stdin, 0);
	close(data->fd_stdin);
}

/**
 * @brief Opens infile. If here_doc is present, infile actually
 * is the limiter. In that case, creates a temporary file and
 * calls read_here_doc to fill the file just like a regular one.
 * 
 * @param data Pointer to the data structure.
 * @param infile Name of the input file.
 */
void	get_infile(t_data *data, char *infile)
{
	if (data->here_doc)
	{
		data->fd_in = open(TMP, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (data->fd_in == -1)
			exit_error(data, "open: unexpected error", 0, 1);
		read_here_doc(data, infile);
		close(data->fd_in);
		data->fd_in = open(TMP, O_RDONLY);
		if (data->fd_in == -1)
			exit_error(data, "open: unexpected error", 0, 1);
	}
	else
	{
		data->fd_in = open(infile, O_RDONLY);
		if (data->fd_in == -1)
		{
			ft_putstr_fd("bash: ", STDERR_FILENO);
			ft_putstr_fd(infile, STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
			ft_putstr_fd(strerror(errno), STDERR_FILENO);
			ft_putchar_fd('\n', STDERR_FILENO);
		}
	}
	return ;
}

/**
 * @brief If here_doc is present, opens outfile in append mode,
 * truncate mode otherwise.
 * 
 * @param data Pointer to the data structure.
 * @param outfile Name of the output file.
 */
void	get_outfile(t_data *data, char *outfile)
{
	if (data->here_doc)
		data->fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		data->fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->fd_out == -1)
	{
		ft_putstr_fd("bash: ", STDERR_FILENO);
		ft_putstr_fd(outfile, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
	}
}

/**
 * @brief Initializes a data structure and parses the
 * argument to get relevant information. Checks the
 * presence of here_doc. Parses each command and finds
 * usable paths. Generates a pipe for each children
 * interaction. Gets the input and output files.
 * 
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 * @param envp Environment.
 * 
 * @return Filled data structure.
 */
t_data	data_init(int argc, char **argv, char **envp)
{
	t_data	data;
	int		i;

	data = data_new();
	if (!ft_strncmp(argv[1], "here_doc", 9))
		data.here_doc = 1;
	get_infile(&data, argv[data.here_doc + 1]);
	argv_parsing(&data, argc, argv);
	data.envp = envp;
	data.pids = (int *)malloc(data.children * sizeof(int));
	data.pipe = (int *)malloc((2 * (data.children - 1)) * sizeof(int));
	data.found = (int *)malloc(data.children * sizeof(int));
	if (!data.pids || !data.pipe || !data.found)
		exit_error(&data, "malloc: ", strerror(errno), 1);
	envp_parsing(&data);
	i = 0;
	while (i < data.children - 1)
	{
		if (pipe(data.pipe + 2 * i) == -1)
			exit_error(&data, "pipe: ", strerror(errno), 1);
		i++;
	}
	get_outfile(&data, argv[argc - 1]);
	return (data);
}
