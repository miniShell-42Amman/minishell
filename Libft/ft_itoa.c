/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 13:45:31 by oissa             #+#    #+#             */
/*   Updated: 2024/09/06 19:28:50 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	numlen(int n)
{
	size_t	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*a;
	size_t	len;
	long	nb;

	nb = n;
	len = numlen(n);
	a = malloc(sizeof(char) * (len + 1));
	if (a == 0)
		return (NULL);
	if (nb < 0)
	{
		a[0] = '-';
		nb = -nb;
	}
	a[len--] = '\0';
	if (nb == 0)
		a[0] = '0';
	while (nb)
	{
		a[len--] = nb % 10 + '0';
		nb /= 10;
	}
	return (a);
}
/*#include <stdio.h>
int main()
{
    int numbers[] = {123, -456, 0, 7890, -2147483648};
    size_t num_tests = sizeof(numbers) / sizeof(numbers[0]);
    char *result;

    for (size_t i = 0; i < num_tests; i++)
    {
        result = ft_itoa(numbers[i]);
        if (result)
        {
            printf("Integer: %d -> String: \"%s\"\n", numbers[i], result);
            free(result); // تحرير الذاكرة المخصصة
        }
        else
        {
            printf("Failed to convert integer: %d\n", numbers[i]);
        }
    }

    return 0;
}*/