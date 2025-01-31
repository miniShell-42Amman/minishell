/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_string.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 23:24:40 by oissa             #+#    #+#             */
/*   Updated: 2024/10/24 16:26:50 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int	ft_printf_string(char *args)
{
	int	length;

	length = 0;
	if (args == NULL)
		return (write(1, "(null)", 6));
	while (args[length] != '\0')
	{
		write(1, &args[length], 1);
		length++;
	}
	return (length);
}
