/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 13:23:34 by ego               #+#    #+#             */
/*   Updated: 2025/02/11 19:50:21 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Parses the argument to get the commands' names and options.
 * 
 * @param data Pointer to the data structure.
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 */
void	argv_parsing(t_data *data, int argc, char **argv)
{
	int	i;

	i = 2 + data->here_doc;
	data->children = argc - 3 - data->here_doc;
	data->cmds = (char ***)malloc((data->children + 1) * sizeof(char **));
	if (!data->cmds)
		exit_error(data, "malloc: ", strerror(errno), 1);
	while (i < argc - 1)
	{
		data->cmds[i - 2 - data->here_doc] = ft_split(argv[i], ' ');
		if (!data->cmds[i - 2 - data->here_doc])
			exit_error(data, "malloc: ", strerror(errno), 1);
		i++;
	}
	data->cmds[i - 2 - data->here_doc] = NULL;
}

/**
 * @brief Searches through the paths to find a usable path
 * for the command of the corresponding child index. Updates
 * the data structure accordingly.
 * 
 * @param data Pointer to the data structure.
 * @param child Child/command index.
 * @param paths All paths found.
 * 
 * @return 1 if a usable path has been found, 0 otherwise.
 */
static int	find_path(t_data *data, int child, char **paths)
{
	int		i;
	char	*path;

	i = 0;
	if (!access(data->cmds[child][0], F_OK | X_OK))
		return (1);
	while (paths[i])
	{
		path = join_strs(paths[i], "/", data->cmds[child][0]);
		if (!path)
		{
			free_split(paths);
			exit_error(data, "malloc: ", strerror(errno), 1);
		}
		if (!access(path, F_OK | X_OK))
		{
			free(data->cmds[child][0]);
			data->cmds[child][0] = path;
			return (1);
		}
		free(path);
		i++;
	}
	return (0);
}

/**
 * @brief Goes through the environment variable to find
 * the PATH= line and splits that line to get the different
 * paths and then checks for each command if it exists.
 * Sets found value to 1 if so, 0 otherwise.
 * 
 * @param data Pointer to the data structure.
 */
void	envp_parsing(t_data *data)
{
	char	**paths;
	int		i;

	i = 0;
	while (ft_strncmp(data->envp[i], "PATH=", 5))
		i++;
	paths = ft_split(data->envp[i] + 5, ':');
	if (!paths)
		exit_error(data, "malloc: ", strerror(errno), 1);
	i = -1;
	while (++i < data->children)
		data->found[i] = find_path(data, i, paths);
	free_split(paths);
}
