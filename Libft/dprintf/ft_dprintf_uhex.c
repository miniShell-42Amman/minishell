/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_hex_uppercase.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 23:24:53 by oissa             #+#    #+#             */
/*   Updated: 2024/09/15 22:17:09 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	ft_dprintf_hex_uppercase(unsigned long n, int fd)
{
	int	len;

	len = 0;
	if (n >= 16)
	{
		len += ft_dprintf_hex_uppercase(n / 16, fd);
		len += ft_dprintf_hex_uppercase(n % 16, fd);
	}
	else
		len += write(fd, &"0123456789ABCDEF"[n], 1);
	return (len);
}
