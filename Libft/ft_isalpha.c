/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:04:52 by oissa             #+#    #+#             */
/*   Updated: 2024/09/05 09:21:16 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int n)
{
	return ((n >= 'a' && n <= 'z') || (n >= 'A' && n <= 'Z'));
}
/*#include <stdio.h>
int main()
{
    // Test cases
    int test_cases[] = {'a', 'z', 'A', 'Z', 'm', 'M', '1', '!', ' ', '\n'};
    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

    // Run the test cases
    for (int i = 0; i < num_tests; i++)
    {
        int result = ft_isalpha(test_cases[i]);
        printf("ft_isalpha('%c') = %d\n", test_cases[i], result);
    }

    return 0;
}*/