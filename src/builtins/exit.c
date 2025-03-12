/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 00:09:27 by oissa             #+#    #+#             */
/*   Updated: 2025/03/12 21:48:44 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **args, int *must_exit)
{
	int	argc;

	argc = 0;
	*must_exit = 1;
	while (args[argc])
		argc++;
	if (argc == 1)
		return (0);
	else if (argc == 2)
	{
		if (!is_numeric(args[1]))
		{
			ft_dprintf(STDERR_FILENO,
				"Error404: exit: %s: numeric argument required\n", args[1]);
			return (2);
		}
		else
			return (ft_atoi(args[1]));
	}
	ft_dprintf(STDERR_FILENO, "exit: too many arguments\n");
	*must_exit = 0;
	return (1);
}
