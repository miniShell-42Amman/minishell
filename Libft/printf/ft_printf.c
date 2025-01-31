/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 23:24:07 by oissa             #+#    #+#             */
/*   Updated: 2024/10/24 16:26:31 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static int	ft_printf_check(char format, va_list args)
{
	int	len;

	len = 0;
	if (format == 's')
		len += ft_printf_string(va_arg(args, char *));
	else if (format == 'd' || format == 'i')
		len += ft_printf_number(va_arg(args, int));
	else if (format == 'u')
		len += ft_printf_unsigned(va_arg(args, unsigned int));
	else if (format == 'x')
		len += ft_printf_hex(va_arg(args, unsigned int));
	else if (format == 'X')
		len += ft_printf_hex_uppercase(va_arg(args, unsigned int));
	else if (format == 'p')
		len += ft_printf_ptr((unsigned long)va_arg(args, void *));
	else if (format == 'c')
		len += ft_printf_char(va_arg(args, int));
	else if (format == '%')
		len += ft_printf_char('%');
	return (len);
}

int	ft_printf(const char *format, ...)
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
			length += ft_printf_check(format[i + 1], args);
			if (length < 0)
				return (length);
			i++;
		}
		else if (format[i] != '%' && format[i])
			length += write(1, &format[i], 1);
		i++;
	}
	va_end(args);
	return (length);
}

// #include <stdio.h>
// int	main ()
// {
// 	ft_printf("%d", ft_printf("%s", "test ft_printf and orginal printf\n"));
// 	printf("%d", ft_printf("%s", "test ft_printf and orginal printf\n"));
// }
// // int	main(void)
// {
// 	printf("len = %d\n", ft_printf(" %p %p ", LONG_MIN, LONG_MAX));
// 	printf("len = %d\n", printf(" %p %p ", LONG_MIN, LONG_MAX));
// }
/*#include <stdio.h>
int main()
{
	ft_printf("***********Testing basic strings:***************************\n");
    char *str = "Hello, World!";
    ft_printf("ft_printf: %s\n", str);
    printf("printf: %s\n", str);
    printf("Testing basic strings:\n");
    // printf("printf: ", "Hello, World!");
    ft_printf("ft_printf: ", "Hello, World!");
    printf("printf: %s %c %d\n", "Hello, World!", 'c', 42);
    ft_printf("ft_printf: %s %c %d\n", "Hello, World!", 'c' , 42);
    printf("\nTesting characters:\n");
    printf("printf: %c %c %c\n", 'a', 'b', 'c');
    ft_printf("ft_printf: %c %c %c\n", 'a', 'b', 'c');
    printf("\nTesting integers:\n");
    printf("printf: %d %d %d\n", 42, -42, 0);
    ft_printf("ft_printf: %d %d %d\n", 42, -42, 0);~
    printf("\nTesting unsigned integers:\n");
    printf("printf: %u %u %u\n", 42, UINT_MAX, 0);
    ft_printf("ft_printf: %u %u %u\n", 42, UINT_MAX, 0);
    printf("\nTesting hexadecimal:\n");
    printf("printf: %x %X\n", 255, 255);
    ft_printf("ft_printf: %x %X\n", 255, 255);
    printf("\nTesting pointers:\n");
    int x = 42;
    printf("printf: %p\n", (void *)&x);
    ft_printf("ft_printf: %p\n", (void *)&x);
    printf("\nTesting percent sign:\n");
    printf("printf: %%\n");
    ft_printf("ft_printf: %%\n");
    return (0);
}*/