/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:45:37 by lalhindi          #+#    #+#             */
/*   Updated: 2025/02/20 20:14:23 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_delim(char c, const char *delim)
{
	while (*delim)
	{
		if (c == *delim)
			return (1);
		delim++;
	}
	return (0);
}

int	ft_strtok_utils(const char *delim, char **start, char **last)
{
	*start = *last;
	while (**last)
	{
		if (is_delim(**last, delim))
		{
			**last = '\0';
			(*last)++;
			if (*start == *last)
			{
				(*start)++;
				continue ;
			}
			return (EXIT_FAILURE);
		}
		(*last)++;
	}
	return (EXIT_SUCCESS);
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*last = NULL;
	char		*start;

	if (str)
		last = str;
	if (!last)
		return (NULL);
	start = last;
	if (ft_strtok_utils(delim, &start, &last) == EXIT_FAILURE)
		return (start);
	last = NULL;
	if (start == last)
		return (NULL);
	return (start);
}
