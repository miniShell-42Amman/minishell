/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 20:10:32 by oissa             #+#    #+#             */
/*   Updated: 2025/01/31 21:37:17 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char ft_strtok(char *str, char *delim)
{
    static char *s;
    char *p;
    char c;

    if (str != NULL)
        s = str;
    if (s == NULL || *s == '\0')
        return '\0';
    while (*s != '\0' && (p = ft_strchr(delim, *s)) != NULL)
        s++;
    if (*s == '\0')
        return '\0';
    c = *s;
    if ((p = ft_strchr(delim, *s)) != NULL)
        s = p;
    else
        s += ft_strlen(s);
    return c;
}

char *trim_spaces(char *str)
{
    while (*str == ' ')
        str++;

    char *end = str + ft_strlen(str) - 1;
    while (end > str && *end == ' ')
        *end-- = '\0';

    return str;
}

int count_args(char *input)
{
    int count = 0;
    bool in_quotes = false;
    char quote_char = '\0';

    for (int i = 0; input[i]; i++)
    {
        if ((input[i] == '"' || input[i] == '\'') && (i == 0 || input[i - 1] != '\\'))
        {
            if (!in_quotes)
            {
                in_quotes = true;
                quote_char = input[i];
            }
            else if (input[i] == quote_char)
                in_quotes = false;
        }
        if (!in_quotes && (input[i] == ' ' || input[i + 1] == '\0'))
            count++;
    }
    return count;
}

t_cmd parse_cmd(char *input)
{
    t_cmd cmd;
    ft_bzero(&cmd, sizeof(t_cmd));
    char *clean_input = ft_strdup(trim_spaces(input));
    int i = 0, j = 0;
    bool in_quotes = false;
    char quote_char = '\0';
    char buffer[1024];

    cmd.arg_count = count_args(clean_input);
    cmd.args = malloc((cmd.arg_count + 1) * sizeof(char *));
    if (!cmd.args)
        exit(1);

    for (int k = 0; clean_input[k]; k++)
    {
        if ((clean_input[k] == '"' || clean_input[k] == '\'') && (k == 0 || clean_input[k - 1] != '\\'))
        {
            if (!in_quotes)
            {
                in_quotes = true;
                quote_char = clean_input[k];
                continue;
            }
            else if (clean_input[k] == quote_char)
            {
                in_quotes = false;
            }
            continue;
        }

        if (!in_quotes && clean_input[k] == ' ')
        {
            if (j > 0)
            {
                buffer[j] = '\0';
                cmd.args[i++] = ft_strdup(buffer);
                j = 0;
            }
        }
        else
        {
            buffer[j++] = clean_input[k];
        }
    }

    if (in_quotes)
    {
        ft_printf(RED "Error: " RESET "Unclosed quotes\n");
        if (clean_input)
            free(clean_input);
        for (int i = 0; i < cmd.arg_count; i++)
            free(cmd.args[i]);
        if (cmd.args)
            free(cmd.args);
        cmd.args = NULL;
        cmd.cmd = NULL;
        cmd.arg_count = 0;
        return cmd;
    }

    if (j > 0)
    {
        buffer[j] = '\0';
        cmd.args[i++] = ft_strdup(buffer);
    }

    cmd.args[i] = NULL;
    cmd.cmd = cmd.args[0] ? ft_strdup(cmd.args[0]) : NULL;
    if (clean_input)
        free(clean_input);
    return cmd;
}

void free_command(t_cmd *cmd)
{
    if (cmd->cmd)
        free(cmd->cmd);  
    if (cmd->args)
    {
        for (int i = 0; i < cmd->arg_count; i++)
            free(cmd->args[i]);
        free(cmd->args);
    }
}

int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
    (void)env;
    char *input;
    t_cmd cmd;

    // ft_bzero(&cmd, sizeof(t_cmd));
    while (1)
    {
        ft_bzero(&cmd, sizeof(t_cmd));
        
        input = readline("minishell$ ");
        if (input == NULL)
            break;
        if (ft_strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }
        if (*input)
        {
            add_history(input);
            cmd = parse_cmd(input);
            if (cmd.args != NULL)
            {
                ft_printf("cmd: %s\n", cmd.cmd);
                for (int i = 0; i < cmd.arg_count; i++)
                {
                    if (cmd.args[i])
                        ft_printf("args[%d]: %s\n", i, cmd.args[i]);
                }
                free_command(&cmd);
            }
        }
        if (input)
            free(input);
    }
    return 0;
}