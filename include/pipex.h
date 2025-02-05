/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:00:14 by ego               #+#    #+#             */
/*   Updated: 2025/02/05 02:05:20 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define TMP ".tmp"
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 50
# endif
# define FD_MAX 1024

# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

typedef struct s_data
{
	char	***cmds;
	char	**envp;
	int		*pids;
	int		*pipe;
	int		child;
	int		here_doc;
	int		fd_in;
	int		fd_out;
}	t_data;

// Data initialization
t_data	data_init(int argc, char **argv, char **envp);

// String utilities
int		ft_char_in_str(char c, char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*join_strs(char *s1, char *s2, char *s3);
char	**ft_split(const char *s, char c);
int		ft_strlen(const char *s);

// Utils
int		free_strs(char *s1, char *s2, char *s3);
char	**free_split(char **s);
void	free_cmds(char ***s);
void	free_data(t_data *data);
void	exit_error(t_data *data, const char *msg1, const char *msg2);

// get_next_line
char	*get_next_line(int fd, int *error);
char	*ft_strdup(const char *s, int *error);
char	*ft_strjoin(const char *s1, const char *s2, int *error);
void	*ft_calloc(size_t nmemb, size_t size, int *error);

// Display
void	ft_putstr(const char *s);
void	ft_putstr_error(const char *s);

#endif