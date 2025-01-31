/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_unsigned.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 23:24:38 by oissa             #+#    #+#             */
/*   Updated: 2024/10/24 16:26:49 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int	ft_printf_unsigned(unsigned int n)
{
	int	len;

	len = 0;
	if (n <= UINT_MAX)
	{
		if (n >= 10)
		{
			len += ft_printf_unsigned(n / 10);
			len += ft_printf_unsigned(n % 10);
		}
		else
			len += ft_printf_char(n + '0');
		return (len);
	}
	return (0);
}
