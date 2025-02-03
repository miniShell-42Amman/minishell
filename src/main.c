/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 20:10:32 by oissa             #+#    #+#             */
/*   Updated: 2025/02/03 19:06:27 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type determine_token_type(char *token, int token_index, t_token *tokens_list)
{
    if (ft_strcmp(token, "|") == 0)
        return (TOKEN_PIPE);
    else if (ft_strcmp(token, "<") == 0)
        return (TOKEN_REDIRECTION_IN);
    else if (ft_strcmp(token, ">") == 0)
        return (TOKEN_REDIRECTION_OUT);
    else if (ft_strcmp(token, ">>") == 0)
        return (TOKEN_REDIRECTION_APPEND);
    else if (ft_strcmp(token, "<<") == 0)
        return (TOKEN_REDIRECTION_HEREDOC);
    else if (token_index == 0 ||(token_index > 0 && tokens_list[token_index - 1].type == TOKEN_PIPE))
        return (TOKEN_COMMAND);
    else
        return (TOKEN_ARGUMENT);
}


t_token *store_token(char **tokens_list, int token_count)
{
    t_token *new_token;
    int i = 0;

    new_token = malloc(sizeof(t_token) * token_count);
    if (!new_token)
        return NULL;
    for (i = 0; i < token_count; i++) 
    {
        if (tokens_list[i] == NULL)
        {
            new_token[i].value = NULL;
            new_token[i].type = TOKEN_ARGUMENT;
            continue;
        }
        new_token[i].value = ft_strdup(tokens_list[i]); 
        if (new_token[i].value == NULL)
        {
            // Handle memory allocation failure
            for (int j = 0; j < i; j++)
                free(new_token[j].value);
            free(new_token);
            return NULL;
        }
        new_token[i].type = determine_token_type(tokens_list[i], i, new_token);
    }
    return new_token;
}
void add_node(t_env **head, t_env *new_node)
{
    t_env *current;

    if (*head == NULL)
    {
        *head = new_node;
        return;
    }
    current = *head;
    while (current->next != NULL)
        current = current->next;
    current->next = new_node;
}

void get_env(char **env)
{
    int i = 0;
    t_env *env_list;
    t_env *new_node;

    while (env[i])
    {
        new_node = malloc(sizeof(t_env));
        if (!new_node)
            return;
        int len_for_key = 0;
        while (env[i][len_for_key]/* && env[i][len_for_key] != '='*/)
        {
            if (env[i][len_for_key] == '=')
            {
                new_node->in_init = true;
                break;
            }
            len_for_key++;
        }
        
        new_node->key = ft_substr(env[i], 0, len_for_key);
        if (new_node->in_init == true)
            new_node->value = ft_strdup(env[i] + len_for_key + 1);
        else
            new_node->value = NULL;
        new_node->next = NULL;

        if (new_node->key == NULL || new_node->value == NULL)
        {
            // Handle memory allocation failure
            
            free(new_node->key);
            free(new_node->value);
            free(new_node);
            return;
        }
        add_node(&env_list, new_node);
        i++;
    }
    // while (env_list)
    // {
    //     ft_printf("Key: %s\n", env_list->key);
    //     ft_printf("Value: %s\n", env_list->value);
    //     ft_printf("bool in_init: %d\n", env_list->in_init);
    //     env_list = env_list->next;
    // }
}

int main(int ac, char **av, char **env)
{
    char *input;
    // char *env = getenv("PATH");
    // ft_printf("PATH: %s\n", env);
    t_cmd cmd;
    t_token *tokens_list;

    (void)ac;
    (void)av;
    // (void)env;
    get_env(env);    
    while (1)
    {
        
        // ft_printf("0 Command: \n");
        // ft_printf("1 Argument: \n");
        // ft_printf("2 REDIRECTION_IN \n");
        // ft_printf("3 REDIRECTION_OUT \n");
        // ft_printf("4 REDIRECTION_APPEND \n");
        // ft_printf("5 REDIRECTION_HEREDOC \n");
        // ft_printf("6 PIPE \n");
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
            rl_clear_history();
            break;
        }
        if (*input)
        {
            add_history(input);
            cmd = parse_cmd(input);
            // int i = 0;
            if (cmd.args != NULL)
            {
                if (cmd.cmd)
                    ft_printf("cmd: %s\n", cmd.cmd);
                tokens_list = store_token(cmd.args, cmd.arg_count);
                
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
        if (input)
            free(input);
    }
    return 0;
}
