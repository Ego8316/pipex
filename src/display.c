/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:10:59 by ego               #+#    #+#             */
/*   Updated: 2025/02/06 19:37:30 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
	return ;
}

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

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
	{
		ft_putstr_fd("-2147483648", fd);
		return ;
	}
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n *= -1;
	}
	if (n >= 10)
	{
		ft_putnbr_fd(n / 10, fd);
		n %= 10;
	}
	ft_putchar_fd('0' + n, fd);
	return ;
}

int	put_help_message(void)
{
	ft_putstr_fd(HELP1, STDERR_FILENO);
	ft_putstr_fd(HELP2, STDERR_FILENO);
	return (1);
}

int	put_bash_warning(const char *limiter)
{
	ft_putstr_fd("\nbash: warning: here-document at line 1", STDOUT_FILENO);
	ft_putstr_fd(" delimited by end-of-file (wanted `", STDOUT_FILENO);
	ft_putstr_fd(limiter, STDOUT_FILENO);
	ft_putstr_fd("')\n", STDOUT_FILENO);
	return (1);
}
