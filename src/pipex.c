/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:07:21 by ego               #+#    #+#             */
/*   Updated: 2025/02/05 01:40:52 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	print_cmds(char ***cmds)
{
	int	i;
	int	j;

	i = 0;
	while (cmds[i])
	{
		j = 0;
		printf("cmd%i:\t", i + 1);
		while (cmds[i][j])
		{
			printf("\"%s\"\t", cmds[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
	return ;
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	data = data_parsing(argc, argv, envp);
	print_cmds(data.cmds);
	free_data(&data);
	return (0);
}
