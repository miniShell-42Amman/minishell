/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 23:49:04 by oissa             #+#    #+#             */
/*   Updated: 2025/02/01 23:49:04 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void init_parse_cmd(t_parse_cmd *parse_cmd, char *input)
{
    ft_bzero(parse_cmd, sizeof(t_parse_cmd));
    ft_bzero(&parse_cmd->cmd, sizeof(t_cmd));
    char *trimmed_input = ft_strtrim(input, " \t\n");
    parse_cmd->clean_input = ft_strdup(trimmed_input);
    free(trimmed_input);
    parse_cmd->buffer = ft_calloc((ft_strlen(parse_cmd->clean_input) + 1), sizeof(char));
    if (!parse_cmd->buffer)
        exit(1);
    parse_cmd->cmd.arg_count = count_args(parse_cmd->clean_input);
    ft_printf("arg_count: %d\n", parse_cmd->cmd.arg_count);
    parse_cmd->cmd.args = ft_calloc((parse_cmd->cmd.arg_count + 2), sizeof(char *));
    if (!parse_cmd->cmd.args)
        exit(1);
}

t_cmd parse_cmd(char *input, t_env *env_list)
{
    t_parse_cmd parse_cmd;
    init_parse_cmd(&parse_cmd, input);
    parse_cmd.token_quote_type = '\0';
    bool token_was_single_quoted = false;
    bool token_was_dollar_quote = false;

    while (parse_cmd.clean_input[parse_cmd.k])
    {
        char c = parse_cmd.clean_input[parse_cmd.k];

        if (!parse_cmd.token_started && c == '$' && parse_cmd.clean_input[parse_cmd.k + 1] == '"')
        {
            token_was_dollar_quote = true;
            parse_cmd.k++;
            continue;
        }

        if ((c == '\'' || c == '"') && parse_cmd.token_quote_type == '\0')
        {
            parse_cmd.token_quote_type = c;
            parse_cmd.in_quotes = true;
            parse_cmd.token_started = true;
            if (c == '\'')
                token_was_single_quoted = true;
            parse_cmd.k++;
            continue;
        }
        else if (c == parse_cmd.token_quote_type && parse_cmd.in_quotes)
        {
            parse_cmd.in_quotes = false;
            parse_cmd.token_quote_type = '\0';
            parse_cmd.k++;
            continue;
        }
        if (!parse_cmd.in_quotes && (c == ' ' || c == '|'))
        {
            if (parse_cmd.token_started)
            {
                parse_cmd.buffer[parse_cmd.j] = '\0';
                if (token_was_single_quoted || token_was_dollar_quote)
                    parse_cmd.cmd.args[parse_cmd.i++] = ft_strdup(parse_cmd.buffer);
                else
                    parse_cmd.cmd.args[parse_cmd.i++] = expand_env_variables_in_token(parse_cmd.buffer, env_list);

                parse_cmd.j = 0;
                parse_cmd.token_started = false;
                parse_cmd.token_quote_type = '\0';
                token_was_single_quoted = false;
                token_was_dollar_quote = false;
            }
            if (c == '|')
            {
                parse_cmd.cmd.args[parse_cmd.i++] = ft_strdup("|");
            }
            parse_cmd.k++;
            continue;
        }
        else
        {
            parse_cmd.buffer[parse_cmd.j++] = c;
            parse_cmd.token_started = true;
        }
        parse_cmd.k++;
    }

    if (parse_cmd.token_started)
    {
        parse_cmd.buffer[parse_cmd.j] = '\0';
        if (token_was_single_quoted || token_was_dollar_quote)
            parse_cmd.cmd.args[parse_cmd.i++] = ft_strdup(parse_cmd.buffer);
        else
            parse_cmd.cmd.args[parse_cmd.i++] = expand_env_variables_in_token(parse_cmd.buffer, env_list);
    }

    if (parse_cmd.in_quotes)
    {
        ft_printf(RED "Error: " RESET "Unclosed quotes\n");
        if (parse_cmd.clean_input)
            free(parse_cmd.clean_input);
        for (int i = 0; i < parse_cmd.cmd.arg_count; i++)
            free(parse_cmd.cmd.args[i]);
        if (parse_cmd.cmd.args)
            free(parse_cmd.cmd.args);
        if (parse_cmd.buffer)
            free(parse_cmd.buffer);
        parse_cmd.cmd.args = NULL;
        parse_cmd.cmd.cmd = NULL;
        parse_cmd.cmd.arg_count = 0;
        return parse_cmd.cmd;
    }

    parse_cmd.cmd.args[parse_cmd.i] = NULL;
    if (parse_cmd.cmd.args[0])
        parse_cmd.cmd.cmd = ft_strdup(parse_cmd.cmd.args[0]);
    else
        parse_cmd.cmd.cmd = NULL;
    if (parse_cmd.clean_input)
        free(parse_cmd.clean_input);
    if (parse_cmd.buffer)
        free(parse_cmd.buffer);
    return parse_cmd.cmd;
}

// t_cmd parse_cmd(char *input)
// {
//     t_parse_cmd parse_cmd;
//     init_parse_cmd(&parse_cmd, input);
//     while (parse_cmd.clean_input[parse_cmd.k])
//     {
//         if (parse_cmd.clean_input[parse_cmd.k] == '"' || parse_cmd.clean_input[parse_cmd.k] == '\'')
//         {
//             if (!parse_cmd.in_quotes)
//             {
//                 parse_cmd.in_quotes = true;
//                 parse_cmd.quote_char = parse_cmd.clean_input[parse_cmd.k];
//                 parse_cmd.token_started = true;
//                 parse_cmd.k++;
//                 continue;
//             }
//             else if (parse_cmd.clean_input[parse_cmd.k] == parse_cmd.quote_char)
//             {
//                 parse_cmd.in_quotes = false;
//                 parse_cmd.k++;
//                 continue;
//             }
//         }
//         if (!parse_cmd.in_quotes && parse_cmd.clean_input[parse_cmd.k] == ' ')
//         {
//             if (parse_cmd.token_started)
//             {
//                 parse_cmd.buffer[parse_cmd.j] = '\0';
//                 parse_cmd.cmd.args[parse_cmd.i++] = expand_env_variables_in_token(parse_cmd.buffer);
//                 if (parse_cmd.cmd.args[parse_cmd.i - 1] == NULL)
//                 {
//                     free(parse_cmd.buffer);
//                     free(parse_cmd.clean_input);
//                     for (int i = 0; i < parse_cmd.cmd.arg_count; i++)
//                         free(parse_cmd.cmd.args[i]);
//                     free(parse_cmd.cmd.args);
//                     parse_cmd.cmd.args = NULL;
//                     parse_cmd.cmd.cmd = NULL;
//                     parse_cmd.cmd.arg_count = 0;
//                     return parse_cmd.cmd;
//                 }
//                 parse_cmd.j = 0;
//                 parse_cmd.token_started = false;
//             }
//             parse_cmd.k++;
//             continue;
//         }
//         else
//         {
//             parse_cmd.buffer[parse_cmd.j++] = parse_cmd.clean_input[parse_cmd.k];
//             parse_cmd.token_started = true;
//         }
//         parse_cmd.k++;
//     }
//     if (parse_cmd.token_started)
//     {
//         parse_cmd.buffer[parse_cmd.j] = '\0';
//         parse_cmd.cmd.args[parse_cmd.i++] = expand_env_variables_in_token(parse_cmd.buffer);
//     }
//     if (parse_cmd.in_quotes)
//     {
//         ft_printf(RED "Error: " RESET "Unclosed quotes\n");
//         if (parse_cmd.clean_input)
//             free(parse_cmd.clean_input);
//         for (int i = 0; i < parse_cmd.cmd.arg_count; i++)
//             free(parse_cmd.cmd.args[i]);
//         if (parse_cmd.cmd.args)
//             free(parse_cmd.cmd.args);
//         if (parse_cmd.buffer)
//             free(parse_cmd.buffer);
//         parse_cmd.cmd.args = NULL;
//         parse_cmd.cmd.cmd = NULL;
//         parse_cmd.cmd.arg_count = 0;
//         return parse_cmd.cmd;
//     }
//     parse_cmd.cmd.args[parse_cmd.i] = NULL;
//     if (parse_cmd.cmd.args[0])
//         parse_cmd.cmd.cmd = ft_strdup(parse_cmd.cmd.args[0]);
//     else
//         parse_cmd.cmd.cmd = NULL;
//     if (parse_cmd.clean_input)
//         free(parse_cmd.clean_input);
//     if (parse_cmd.buffer)
//         free(parse_cmd.buffer);
//     return (parse_cmd.cmd);
// }
