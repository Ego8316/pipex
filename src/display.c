/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ego <ego@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:10:59 by ego               #+#    #+#             */
/*   Updated: 2025/02/04 20:27:26 by ego              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_putstr(const char *s)
{
	int	len;

	len = 0;
	while (s[len])
		len++;
	write(1, s, len);
	return ;
}

void	ft_putstr_error(const char *s)
{
	int	len;

	len = 0;
	while (s[len])
		len++;
	write(2, s, len);
	return ;
}
