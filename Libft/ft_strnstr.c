/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 11:28:38 by oissa             #+#    #+#             */
/*   Updated: 2024/09/01 15:21:05 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s, const char *need, size_t len)
{
	size_t	i;
	size_t	j;

	j = 0;
	i = 0;
	if (need[0] == '\0')
	{
		return ((char *) s);
	}
	while (s[i] != '\0' && i < len)
	{
		while (s[i + j] == need [j] && i + j < len)
		{
			if (need[j + 1] == '\0')
			{
				return ((char *) &s[i]);
			}
			j ++;
		}
		j = 0;
		i++;
	}
	return (0);
}
