/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_number.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 23:24:45 by oissa             #+#    #+#             */
/*   Updated: 2024/10/24 16:26:54 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int	ft_printf_number(int n)
{
	char	*a;
	int		len;

	len = 0;
	a = ft_itoa(n);
	len += ft_printf_string(a);
	free(a);
	return (len);
}
