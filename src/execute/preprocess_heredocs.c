/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_heredocs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:49:00 by oissa             #+#    #+#             */
/*   Updated: 2025/03/09 23:55:40 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int preprocess_heredocs(t_execute *execute, t_main *main)
{
    t_redirections redirections;
    size_t i;
    int num_command;
    int tmp = dup(STDIN_FILENO);
    if (tmp < 0)
    {
        perror("dup");
        return (EXIT_FAILURE);
    }
    i = -1;
    num_command = 0;
    while (++i < execute->num_cmds)
    {
        ft_bzero(&redirections, sizeof(t_redirections));
        redirections.argv = execute->commands[i];
        redirections.j = 0;
        while (redirections.argv[redirections.j])
        {
            if (main->tokens_list[num_command].value && main->tokens_list[num_command].type == TOKEN_PIPE)
                num_command++;
            if (ft_strcmp(redirections.argv[redirections.j], "<<") == 0 && main->tokens_list[num_command].type != TOKEN_ARGUMENT)
            {
                signal(SIGINT, handle_heredoc_sigint);
                if (redirection_check_else_if(&redirections, execute, main) != EXIT_SUCCESS)
                    return (EXIT_FAILURE);
                if (g_signal == 130)
                {
                    free_redirections(&redirections);
                    free_execute(execute, 0);
                    break;
                }
            }
            else
            {
                redirections.j++;
                num_command++;
            }
        }
        if (g_signal == 130)
        {
            dup2(tmp, STDIN_FILENO);
            close(tmp);
            signal(SIGINT, handle_sigint);
            return (EXIT_FAILURE);
        }
    }
    close(tmp);
    signal(SIGINT, handle_sigint);
    return (EXIT_SUCCESS);
}
