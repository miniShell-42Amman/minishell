/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 23:24:34 by oissa             #+#    #+#             */
/*   Updated: 2024/10/24 16:35:29 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <limits.h>
# include <stdarg.h>
# include <stdlib.h>
# include <stddef.h>
# include <unistd.h>
# include "libft.h"

int		ft_printf(const char *formater, ...);
int		ft_printf_char(char c);
int		ft_printf_string(char *args);
int		ft_printf_unsigned(unsigned int n);
size_t	ft_printf_hex(unsigned int n);
size_t	ft_printf_hex_uppercase(unsigned int n);
int		ft_printf_number(int n);
int		ft_printf_ptr(unsigned long n);
char	*ft_itoa(int n);
size_t	ft_printf_hex_putptr(unsigned long n);

#endif