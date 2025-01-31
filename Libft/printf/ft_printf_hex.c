/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_hex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 23:24:48 by oissa             #+#    #+#             */
/*   Updated: 2024/10/24 16:26:56 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

size_t	ft_printf_hex_putptr(unsigned long n)
{
	size_t	len;

	len = 0;
	if (n >= 16)
	{
		len += ft_printf_hex_putptr(n / 16);
		len += ft_printf_hex_putptr(n % 16);
	}
	else
		len += write(1, &"0123456789abcdef"[n], 1);
	return (len);
}

size_t	ft_printf_hex(unsigned int n)
{
	size_t	len;

	len = 0;
	if (n >= 16)
	{
		len += ft_printf_hex(n / 16);
		len += ft_printf_hex(n % 16);
	}
	else
		len += write(1, &"0123456789abcdef"[n], 1);
	return (len);
}
// 254 => fe
// int main()
// {
//     unsigned long num = 254;
//     int length = printf_hex(num); 
//     write(1, "\n", 1); 
//     write(1, "Number of characters printed: ", 30);    
//     char buffer[20];
//     int len = sprintf(buffer, "%d\n", length);
//     write(1, buffer, len);
//     return 0;
// }