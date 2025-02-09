/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 23:24:07 by oissa             #+#    #+#             */
/*   Updated: 2024/11/25 13:27:53 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

static int	ft_dprintf_check(char format, va_list args, int fd)
{
	int	len;

	len = 0;
	if (format == 's')
		len += ft_dprintf_string(va_arg(args, char *), fd);
	else if (format == 'd' || format == 'i')
		len += ft_dprintf_number(va_arg(args, int), fd);
	else if (format == 'u')
		len += ft_dprintf_unsigned(va_arg(args, unsigned int), fd);
	else if (format == 'x')
		len += ft_dprintf_hex(va_arg(args, unsigned long), fd);
	else if (format == 'X')
		len += ft_dprintf_hex_uppercase(va_arg(args, unsigned long), fd);
	else if (format == 'p')
		len += ft_dprintf_ptr(va_arg(args, unsigned long long), fd);
	else if (format == 'c')
		len += ft_dprintf_char(va_arg(args, int), fd);
	else if (format == '%')
		len += ft_dprintf_char('%', fd);
	return (len);
}

int	ft_dprintf(int fd, const char *format, ...)
{
	va_list	args;
	int		i;
	int		length;

	i = 0;
	length = 0;
	va_start(args, format);
	while (format[i] != '\0')
	{
		if (format[i] == '%' && format[i])
		{
			length += ft_dprintf_check(format[i + 1], args, fd);
			if (length < 0)
				return (length);
			i++;
		}
		else if (format[i] != '%' && format[i])
			length += write(fd, &format[i], 1);
		i++;
	}
	va_end(args);
	return (length);
}
// int main()
// {
// 	char *str = "Hello, World!";
// 	// printf("%s\n", str);
// 	ft_printf_fd(2, "%s\n", str);
// 	ft_printf_fd(1, "%p\n", str);
// }
// #include <stdio.h>
// int main ()
// {
// 	char *str = "Hello, World!";
// 	printf("%s\n", str);
// 	ft_printf("%s\n", str);
// }
// #include <stdio.h>
// int main()
// {
// 	ft_printf("***********Testing basic strings:***************************\n");
//     char *str = "Hello, World!";
//     ft_printf("ft_printf: %s\n", str);
//     printf("printf: %s\n", str);
//     printf("Testing basic strings:\n");
//     // printf("printf: ", "Hello, World!");
//     ft_printf("ft_printf: ", "Hello, World!");
//     printf("printf: %s %c %d\n", "Hello, World!", 'c', 42);
//     ft_printf("ft_printf: %s %c %d\n", "Hello, World!", 'c' , 42);
//     printf("\nTesting characters:\n");
//     printf("printf: %c %c %c\n", 'a', 'b', 'c');
//     ft_printf("ft_printf: %c %c %c\n", 'a', 'b', 'c');
//     printf("\nTesting integers:\n");
//     printf("printf: %d %d %d\n", 42, -42, 0);
//     ft_printf("ft_printf: %d %d %d\n", 42, -42, 0);
//     printf("\nTesting unsigned integers:\n");
//     printf("printf: %u %u %u\n", 42, UINT_MAX, 0);
//     ft_printf("ft_printf: %u %u %u\n", 42, UINT_MAX, 0);
//     printf("\nTesting hexadecimal:\n");
//     printf("printf: %x %X\n", 255, 255);
//     ft_printf("ft_printf: %x %X\n", 255, 255);
//     printf("\nTesting octal:\n");
//     printf("printf: %o\n", 255);
//     ft_printf("ft_printf: %o\n", 255);
//     printf("\nTesting pointers:\n");
//     int x = 42;
//     printf("printf: %p\n", (void *)&x);
//     ft_printf("ft_printf: %p\n", (void *)&x);
//     printf("\nTesting percent sign:\n");
//     printf("printf: %%\n");
//     ft_printf("ft_printf: %%\n");
//     return (0);
// }