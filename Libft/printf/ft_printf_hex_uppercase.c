/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_hex_uppercase.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 23:24:53 by oissa             #+#    #+#             */
/*   Updated: 2024/10/24 16:26:58 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

size_t	ft_printf_hex_uppercase(unsigned int n)
{
	size_t	len;

	len = 0;
	if (n >= 16)
	{
		len += ft_printf_hex_uppercase(n / 16);
		len += ft_printf_hex_uppercase(n % 16);
	}
	else
		len += write(1, &"0123456789ABCDEF"[n], 1);
	return (len);
}
