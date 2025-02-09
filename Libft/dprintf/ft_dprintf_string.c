/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_string.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 23:24:40 by oissa             #+#    #+#             */
/*   Updated: 2024/09/14 14:53:51 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	ft_dprintf_string(char *args, int fd)
{
	int	length;

	length = 0;
	if (args == NULL)
		return (write(fd, "(null)", 6));
	while (args[length] != '\0')
	{
		write(fd, &args[length], 1);
		length++;
	}
	return (length);
}
