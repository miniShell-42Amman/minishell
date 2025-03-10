/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_sep.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:40:40 by oissa             #+#    #+#             */
/*   Updated: 2025/02/09 23:16:11 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>

int	ft_strlen_sep(char *str, int c, bool *has_value)
{
	int	i;

	i = 0;
	while (str[i] || str[i] != c)
	{
		if (str[i] == c)
		{
			*has_value = true;
			break ;
		}
		i++;
	}
	return (i);
}
