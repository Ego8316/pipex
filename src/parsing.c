/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 13:23:34 by ego               #+#    #+#             */
/*   Updated: 2025/02/06 20:11:55 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*	argv_parsing
*	Parses the argument to get the command names and options.
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

/*	find_path
*	Given the data structure, the children index and the paths,
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

/*	envp_parsing
*	Goes through the envp to find the PATH= line
*	and splits that line to get the different paths.
*	Afterwards, checks for each command if it exists
*	and exits the program if it does not.
*/
void	envp_parsing(t_data *data)
{
	char	**paths;
	char	*tmp;
	int		i;

	i = 0;
	while (ft_strncmp(data->envp[i], "PATH=", 5))
		i++;
	paths = ft_split(data->envp[i] + 5, ':');
	if (!paths)
		exit_error(data, "malloc: ", strerror(errno), 1);
	i = -1;
	while (++i < data->children)
	{
		data->found[i] = find_path(data, i, paths);
		if (!data->found[i] && !(i == 0 && data->error_msg))
		{
			tmp = data->error_msg;
			tmp = join_strs(tmp, data->cmds[i][0], ": command not found\n");
			free(data->error_msg);
			if (!tmp)
				exit_error(data, "malloc: ", strerror(errno), 1);
			data->error_msg = tmp;
		}
	}
	free_split(paths);
}
