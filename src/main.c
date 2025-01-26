/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 20:10:32 by oissa             #+#    #+#             */
/*   Updated: 2025/01/26 21:55:52 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main (int ac, char **av)
{
    (void)ac;
    (void)av;
    t_minishell minishell;
    
    char *input;
    while (1)
    {
        input = readline("MiniShell:$ ");
        // if (input && *input)
        // {
        //     // ft_printf("%s\n", input);
        //     free(input);
        // }
        minishell.arg = ft_split(input, ' ');
        int i = 0;
        
        while (minishell.arg[i])
        {
            ft_printf("%s\n", minishell.arg[i]);
            i++;
        }
    }
    return 0;
}