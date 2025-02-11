/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:10:59 by ego               #+#    #+#             */
/*   Updated: 2025/02/11 19:35:04 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Prints a character into given file descriptor.
 * 
 * @param c Character to be printed.
 * @param fd File descriptor.
 */
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
	return ;
}

/**
 * @brief Prints a string into given file descriptor.
 * 
 * @param c Character to be printed.
 * @param fd File descriptor.
 */
void	ft_putstr_fd(const char *s, int fd)
{
	int	len;

	len = 0;
	if (s)
	{
		while (s[len])
			len++;
		write(fd, s, len);
	}
	return ;
}

/**
 * @brief Simply displays a help message in case
 * of argument mistype.
 * 
 * @return 1.
 */
int	put_help_message(void)
{
	ft_putstr_fd(HELP1, STDERR_FILENO);
	ft_putstr_fd(HELP2, STDERR_FILENO);
	return (1);
}

/**
 * @brief Prints a bash warning similar to that of bash
 * when end-of-file is reached before finding the limiter
 * when filling a here-document.
 * 
 * @param limiter Here-doc limiter.
 * 
 * @return 1.
 */
int	put_bash_warning(const char *limiter)
{
	ft_putstr_fd("\nbash: warning: here-document at line 1", STDOUT_FILENO);
	ft_putstr_fd(" delimited by end-of-file (wanted `", STDOUT_FILENO);
	ft_putstr_fd(limiter, STDOUT_FILENO);
	ft_putstr_fd("')\n", STDOUT_FILENO);
	return (1);
}
