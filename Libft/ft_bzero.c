/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:37:56 by oissa             #+#    #+#             */
/*   Updated: 2024/09/05 09:57:23 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t len)
{
	ft_memset(s, 0, len);
}
/*#include <stdio.h>
void	*ft_memset(void *b, int c, size_t len)
{
	char	*p;

	p = (char *)b;
	while (len > 0)
	{
		p[len - 1] = c;
		len--;
	}
	return (b);
}
int main()
{
    char str1[50] = "Hello, World!";
    char str2[50] = "Hello, World!";

    printf("Before ft_bzero: %s\n", str1);
    ft_bzero(str1, 5);
    printf("After ft_bzero: %s\n", str1);

    printf("Before ft_memset: %s\n", str2);
    ft_memset(str2, 'X', 5);
    printf("After ft_memset: %s\n", str2);

    return 0;
}*/