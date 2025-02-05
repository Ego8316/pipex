/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:00:14 by ego               #+#    #+#             */
/*   Updated: 2025/02/05 19:36:18 by ego              ###   ########.fr       */
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
# include <errno.h>

typedef struct s_data
{
	char	***cmds;
	char	**envp;
	char	*errors;
	int		*pids;
	int		*pipe;
	int		child;
	int		here_doc;
	int		fd_in;
	int		fd_out;
	int		fd_stdin;
}	t_data;

// Data initialization
void	argv_parsing(t_data *data, int argc, char **argv);
void	envp_parsing(t_data *data);
t_data	data_init(int argc, char **argv, char **envp);

// String utilities
int		ft_char_in_str(char c, char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*join_strs(char *s1, char *s2, char *s3);
char	**ft_split(const char *s, char c);
int		ft_strlen(const char *s);

// Utils
int		ft_free(char **s);
char	**free_split(char **s);
void	free_cmds(char ***s);
void	free_data(t_data *data);
void	exit_error(t_data *data, char *msg1, char *msg2, int nl);

// get_next_line
char	*get_next_line(int fd, int *error);
char	*ft_strdup(const char *s, int *error);
char	*ft_strjoin(const char *s1, const char *s2, int *error);
void	*ft_calloc(size_t nmemb, size_t size, int *error);

// Display
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(const char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
int		put_bash_warning(const char *limiter);

#endif