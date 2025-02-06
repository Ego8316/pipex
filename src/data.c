/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:48:40 by ego               #+#    #+#             */
/*   Updated: 2025/02/06 19:49:34 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*	data_new
*	Initializes a data structure.
*	Returns: the empty data structure.
*/
static t_data	data_new(void)
{
	t_data	data;

	data.cmds = NULL;
	data.envp = NULL;
	data.error_msg = NULL;
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

/*	read_here_doc
*	Duplicates standard input, reads from it and writes in the
*	.tmp temporary file until limiter is found. If EOF is reached
*	before finding limiter, prints bash warning. get_next_line
*	is called afterwards with NULL pointer to empty the stash.
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

/*	get_infile
*	If here_doc present: infile actually is the limiter.
*	Creates a temporary file and calls read_here_doc
*	to fill the file like a regular one.
*	Otherwise, tries to open infile.
*/

void	get_infile(t_data *data, char *infile)
{
	char	*tmp;

	if (data->here_doc)
	{
		data->fd_in = open(TMP, O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (data->fd_in == -1)
			exit_error(data, "open: unexpected error", 0, 1);
		read_here_doc(data, infile);
	}
	else
	{
		data->fd_in = open(infile, O_RDONLY);
		if (data->fd_in == -1)
		{
			data->error_msg = join_strs("bash: ", infile, ": ");
			if (!data->error_msg)
				exit_error(data, "malloc: ", strerror(errno), 1);
			tmp = join_strs(data->error_msg, strerror(errno), "\n");
			free(data->error_msg);
			if (!tmp)
				exit_error(data, "malloc: ", strerror(errno), 1);
			data->error_msg = tmp;
		}
	}
	return ;
}

/*	get_outfile
*	If here_doc is present, opens outfile in append mode,
*	truncate mode otherwise.
*/
void	get_outfile(t_data *data, char *outfile)
{
	if (data->here_doc)
		data->fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		data->fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->fd_out == -1)
		exit_error(data, "open: unexpected error", 0, 1);
}

/*	data_init
*	Initializes a data structure and parses the argument
*	to get relevant information. Checks the presence of
*	here_doc, if present gets the infile from STDIN.
*	Splits each argument into commands and finds the
*	the usable paths. Generates a pipe for each children
*	interaction.
*	Returns: the filled data structure.
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
	ft_putstr_fd(data.error_msg, STDERR_FILENO);
	return (data);
}
