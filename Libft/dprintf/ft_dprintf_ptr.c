/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_ptr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 23:24:42 by oissa             #+#    #+#             */
/*   Updated: 2024/09/16 21:01:20 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	ft_dprintf_ptr(unsigned long long n, int fd)
{
	int	len;

	len = 0;
	if (n != '\0')
	{
		len += ft_dprintf_char('0', fd);
		len += ft_dprintf_char('x', fd);
		len += ft_dprintf_hex(n, fd);
		return (len);
	}
	len += write(fd, "(nil)", 5);
	return (len);
}
