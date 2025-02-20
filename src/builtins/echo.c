/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 21:15:23 by oissa             #+#    #+#             */
/*   Updated: 2025/02/20 22:43:14 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo(char **args, int arg_count)
{
    bool new_line;
    int i;

    i = 1;
    new_line = true;
    if (arg_count > 1 && ft_strncmp(args[i], "-n", 2) == 0)
    {
        new_line = false;
        i++;
    }
    while (args[i])
    {
        ft_printf("%s", args[i]);
        if (args[i + 1])
            ft_printf(" ");
        i++;
    }
    if (new_line)
        ft_printf("\n");
}
