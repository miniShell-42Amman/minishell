/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 20:10:32 by oissa             #+#    #+#             */
/*   Updated: 2025/02/05 17:39:06 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av, char **env)
{
    char *input;
    t_cmd cmd;
    t_token *tokens_list;
    t_env *env_list;

    (void)ac;
    (void)av;
    env_list = save_env(env);    
    while (1)
    {
        ft_bzero(&cmd, sizeof(t_cmd));   
        ft_printf(YELLOW);
        input = readline("minishell$ > ");
        int *array = ft_count_args(input);
        ft_printf(RESET);
        if (input == NULL)
            break;
        if (ft_strcmp(input, "exit") == 0)
        {
            free(input);
            free_command(&cmd);
            rl_clear_history();
            break;
        }
        if (*input)
        {
            add_history(input);
            cmd = parse_cmd(input, env_list);
            if (cmd.args != NULL)
            {
                if (cmd.cmd)
                    ft_printf("cmd: %s\n", cmd.cmd);
                tokens_list = store_token(cmd.args, cmd.arg_count, array);
                
                if (tokens_list)
                {
                    for (int k = 0; k < cmd.arg_count; k++) {
                       if (tokens_list[k].value)
                       {
                         if (tokens_list[k].value != NULL)
                            printf("Token[%d]: %s => %d\n", k, tokens_list[k].value, tokens_list[k].type);
                        else
                            printf("Token[%d]: (null) => %d\n", k, tokens_list[k].type);
                        free(tokens_list[k].value);
                       }
                    }
                    free(tokens_list);
                }
                free_command(&cmd);
            }
        }
        if (array)
            free(array);
        if (input)
            free(input);
    }
    return 0;
}
