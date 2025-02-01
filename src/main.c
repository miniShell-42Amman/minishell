/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 20:10:32 by oissa             #+#    #+#             */
/*   Updated: 2025/02/02 00:25:35 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"



int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
    (void)env;
    char *input;
    t_cmd cmd;

    while (1)
    {
        char *cwd = getcwd(NULL, 0);
        ft_printf(PURPLE "%s ", cwd);
        ft_bzero(&cmd, sizeof(t_cmd));
        
        ft_printf(YELLOW);
        input = readline("minishell$ > ");
        ft_printf(RESET);
        if (input == NULL)
            break;
        if (ft_strcmp(input, "exit") == 0)
        {
            free(input);
            free_command(&cmd);
            free(cwd);
            break;
        }
        if (*input)
        {
            add_history(input);
            cmd = parse_cmd(input);
            if (cmd.args != NULL)
            {
                if (cmd.cmd)
                    ft_printf("cmd: %s\n", cmd.cmd);
                int i = 0;
                while (i < cmd.arg_count)
                {
                    if (cmd.args[i]) 
                        ft_printf("args[%d]: %s\n", i, cmd.args[i]);
                    i++;
                }
                free_command(&cmd);
            }
        }
        if (input)
            free(input);
        if (cwd)
            free(cwd);
    }
    return 0;
}
