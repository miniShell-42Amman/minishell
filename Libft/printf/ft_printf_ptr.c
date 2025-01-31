/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_ptr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 23:24:42 by oissa             #+#    #+#             */
/*   Updated: 2024/10/24 16:26:52 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int	ft_printf_ptr(unsigned long n)
{
	int	len;

	len = 0;
	if (n != 0)
	{
		len += ft_printf_char('0');
		len += ft_printf_char('x');
		len += ft_printf_hex_putptr(n);
	}
	else
		len += write(1, "(nil)", 5);
	return (len);
}
