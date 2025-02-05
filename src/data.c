/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:48:40 by ego               #+#    #+#             */
/*   Updated: 2025/02/05 02:06:20 by ego              ###   ########.fr       */
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
	data.pids = NULL;
	data.pipe = NULL;
	data.child = 0;
	data.here_doc = 0;
	data.fd_in = -1;
	data.fd_out = -1;
	return (data);
}

/*	argv_parsing
*	Parses the argument to get the command names and options.
*/
static void	argv_parsing(t_data *data, int argc, char **argv)
{
	int	i;

	i = 2 + data->here_doc;
	data->child = argc - 3 - data->here_doc;
	data->cmds = (char ***)malloc((data->child + 1) * sizeof(char **));
	if (!data->cmds)
		exit_error(data, "malloc: failed allocation\n", 0);
	while (i < argc - 1)
	{
		(data->cmds)[i - 2 - data->here_doc] = ft_split(argv[i], ' ');
		if (!((data->cmds)[i - 2 - data->here_doc]))
			exit_error(data, "malloc: failed allocation\n", 0);
		i++;
	}
	(data->cmds)[i - 2 - data->here_doc] = NULL;
}

/*	find_path
*	Given the data structure, the child index and the paths,
*	search through the latter to see if the command can be
*	found in one of them. If so, changes the data structure
*	accordingly.
*	Returns: 1 if the command has been found, 0 otherwise.
*/
static int	find_path(t_data *data, int child, char **paths)
{
	int		i;
	char	*path;

	i = 0;
	while (paths[i])
	{
		path = join_strs(paths[i], "/", (data->cmds)[child][0]);
		if (!path)
		{
			free_split(paths);
			exit_error(data, "malloc: allocation failed", 0);
		}
		if (access(path, F_OK | X_OK))
		{
			free((data->cmds)[child][0]);
			(data->cmds)[child][0] = path;
			return (1);
		}
		free(path);
		i++;
	}
	return (0);
}

/*	envp_parsing
*	Goes through the envp to find the PATH= line
*	and splits that line to get the different paths.
*	Afterwards, checks for each command if it exists
*	and exits the program if it does not.
*/
static void	envp_parsing(t_data *data)
{
	char	**paths;
	int		i;

	i = 0;
	while (ft_strncmp((data->envp)[i], "PATH=", 5))
		i++;
	paths = ft_split((data->envp)[i] + 5, ':');
	if (!paths)
		exit_error(data, "malloc: failed allocation\n", 0);
	i = 0;
	while (i < data->child)
	{
		if (!find_path(data, i, paths))
		{
			free_split(paths);
			exit_error(data, (data->cmds)[i][0], ": command not found\n");
		}
		i++;
	}
	free_split(paths);
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

	data = data_new();
	if (!ft_strncmp(argv[1], "here_doc", 9))
		data.here_doc = 1;
	argv_parsing(&data, argc, argv);
	data.envp = envp;
	envp_parsing(&data);
	data.pids = (int *)malloc(data.child * sizeof(int));
	data.pipe = (int *)malloc(2 * (data.child - 1) * sizeof(int));
	if (!data.pids || !data.pipe)
		exit_error(&data, "malloc: failed allocation\n", 0);
	return (data);
}
