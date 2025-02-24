/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 01:22:35 by lalhindi          #+#    #+#             */
/*   Updated: 2025/02/24 07:58:11 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_n_flag(char *arg)
{
	int	j;

	if (ft_strncmp(arg, "-n", 2) != 0)
		return (0);
	j = 1;
	while (arg[++j])
		if (arg[j] != 'n')
			return (0);
	return (1);
}

static void	print_args(char **args, int i)
{
	while (args[i])
	{
		ft_printf(args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
}

void	echo(char **args)
{
	int	i;
	int	new_line;
	i = 1;
	new_line = 1;
	if (!args[1])
	{
		ft_printf("\n");
		return ;
	}
	
	while (args[i] && check_n_flag(args[i]))
	{
		new_line = 0;
		i++;
	}
	print_args(args, i);
	if (new_line)
		ft_printf("\n");
}
