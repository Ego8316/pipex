/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:48:40 by ego               #+#    #+#             */
/*   Updated: 2025/02/05 14:43:44 by ego              ###   ########.fr       */
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
	data.errors = NULL;
	data.pids = NULL;
	data.pipe = NULL;
	data.child = 0;
	data.here_doc = 0;
	data.fd_in = -1;
	data.fd_out = -1;
	return (data);
}

void	get_infile(t_data *data, char *infile)
{
	char	*tmp;

	if (data->here_doc)
	{
		data->fd_in = open(TMP, O_RDONLY);
		if (data->fd_in == -1)
			exit_error(data, "open: unexpected error", 0, 1);
	}
	else
	{
		data->fd_in = open(infile, O_RDONLY);
		if (data->fd_in == -1)
		{
			data->errors = join_strs("bash: ", infile, ": ");
			if (!data->errors)
				exit_error(data, "malloc: ", strerror(errno), 1);
			tmp = join_strs(data->errors, strerror(errno), "\n");
			free(data->errors);
			if (!tmp)
				exit_error(data, "malloc: ", strerror(errno), 1);
			data->errors = tmp;
		}
	}
}

void	get_here_doc(t_data *data, char *limiter)
{
	limiter[0] = 'a';
	data->here_doc = 1;
}

/*	data_init
*	Initializes a data structure and parses the argument
*	to get relevant information. Checks the presence of
*	here_doc, if present gets the infile from STDIN.
*	Splits each argument into commands and finds the
*	the usable paths. Generates a pipe for each child
*	interaction.
*	Returns: the filled data structure.
*/
t_data	data_init(int argc, char **argv, char **envp)
{
	t_data	data;
	int		i;

	data = data_new();
	if (!ft_strncmp(argv[1], "here_doc", 9))
	{
		data.here_doc = 1;
		get_here_doc(&data, argv[2]);
	}
	get_infile(&data, argv[1]);
	argv_parsing(&data, argc, argv);
	data.envp = envp;
	envp_parsing(&data);
	data.pids = (int *)malloc(data.child * sizeof(int));
	data.pipe = (int *)malloc(2 * (data.child - 1) * sizeof(int));
	if (!data.pids || !data.pipe)
		exit_error(&data, "malloc: ", strerror(errno), 1);
	i = 0;
	while (i < data.child - 1)
	{
		if (pipe(data.pipe + 2 * i) == -1)
			exit_error(&data, "pipe: ", strerror(errno), 1);
		i++;
	}
	if (data.errors)
		exit_error(&data, data.errors, 0, 0);
	return (data);
}
