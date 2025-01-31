/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol_handle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 10:54:21 by oissa             #+#    #+#             */
/*   Updated: 2024/11/16 15:57:47 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// static size_t	handle_initial(const char *str, int *sign)
// {
// 	size_t	i;

// 	i = 0;
// 	*sign = 1;
// 	if (str[i] == '-')
// 		*sign = -1;
// 	if (str[i] == '-' || str[i] == '+')
// 		i++;
// 	while (str[i] == '0')
// 		i++;
// 	return (i);
// }

// static size_t	is_valid_length(const char *str, size_t i)
// {
// 	if (ft_strlen(str + i) <= 10)
// 		return (1);
// 	return (0);
// }

// static long	convert_and_validate(const char *str, size_t i, int sign,
// 		int *valid)
// {
// 	long	result;

// 	result = 0;
// 	while (str[i])
// 	{
// 		if (str[i] < '0' || str[i] > '9')
// 		{
// 			*valid = 0;
// 			return (0);
// 		}
// 		result = result * 10 + (str[i] - '0');
// 		if (result * sign > INT_MAX || result * sign < INT_MIN)
// 		{
// 			*valid = 0;
// 			return (0);
// 		}
// 		i++;
// 	}
// 	return (result * sign);
// }

// long	ft_atol_handle(const char *str, int *valid)
// {
// 	int		sign;
// 	size_t	i;
// 	long	result;

// 	*valid = 1;
// 	i = handle_initial(str, &sign);
// 	if (!is_valid_length(str, i))
// 	{
// 		*valid = 0;
// 		return (0);
// 	}
// 	result = convert_and_validate(str, i, sign, valid);
// 	return (result);
// }
/*
	example:
		char *str = "   +1234"; --> i = 4 --> sign = 1
		char *str = "   -1234"; --> i = 4 --> sign = -1
		char *str = "0000001234"; --> i = 7 --> sign = 1
*/
static size_t	handle_initial(const char *str, int *sign)
{
	size_t	i;

	i = 0;
	*sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			*sign = -1;
		i++;
	}
	while (str[i] == '0')
		i++;
	return (i);
}
/*
	example:
		the length of the number is less than or equal to 10
*/

static int	is_valid_length(const char *str, size_t i)
{
	size_t	len;

	len = 0;
	while (str[i + len] && ft_isdigit(str[i + len]))
		len++;
	if (len <= 10)
		return (1);
	return (0);
}

static long	convert_and_validate(const char *str, size_t i, int sign,
		int *valid)
{
	long	result;
	int		digit;

	result = 0;
	while (str[i] && ft_isdigit(str[i]))
	{
		digit = str[i] - '0';
		if (result > (LONG_MAX - digit) / 10)
		{
			*valid = 0;
			return (0);
		}
		result = result * 10 + digit;
		i++;
	}
	result *= sign;
	if (result > INT_MAX || result < INT_MIN)
	{
		*valid = 0;
		return (0);
	}
	return (result);
}

long	ft_atol_handle(const char *str, int *valid)
{
	int		sign;
	size_t	i;
	long	result;

	*valid = 1;
	i = handle_initial(str, &sign);
	if (!is_valid_length(str, i))
	{
		*valid = 0;
		return (0);
	}
	result = convert_and_validate(str, i, sign, valid);
	return (result);
}
