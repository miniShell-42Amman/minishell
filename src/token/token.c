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
    char *trimmed_input = trim_spaces(input);
    parse_cmd->clean_input = ft_strdup(trimmed_input);
    free(trimmed_input);
    parse_cmd->buffer = ft_calloc((ft_strlen(parse_cmd->clean_input) + 1), sizeof(char));
    if (!parse_cmd->buffer)
        exit(1);
    parse_cmd->cmd.arg_count = count_args(parse_cmd->clean_input);
    parse_cmd->cmd.args = ft_calloc((parse_cmd->cmd.arg_count + 1) , sizeof(char *));
    if (!parse_cmd->cmd.args)
        exit(1);
}

// char *expand_env_variable(char *token)
// {
//     if (token[0] != '$') 
//         return ft_strdup(token);

//     char *var_name = token + 1;
//     char *value = getenv(var_name);

//     if (value)
//         return ft_strdup(value);
//     else
//         return ft_strdup("");
// }

char *expand_env_variables_in_token(char *token) {
    if (token == NULL) return NULL;

    size_t initial_size = ft_strlen(token) * 2 + 1;
    char *result = ft_calloc(initial_size, sizeof(char));
    if (!result) return NULL;

    size_t i = 0, j = 0;
    while (token[i]) {
        if (token[i] == '$') {
            i++;
            if (token[i] == '\0' || isspace(token[i])) {
                result[j++] = '$';
                continue;
            }
            char var_name[256];
            int k = 0;
            while (token[i] && (isalnum(token[i]) || token[i] == '_')) {
                var_name[k++] = token[i++];
            }
            var_name[k] = '\0';
            char *value = getenv(var_name);
            if (value) {
                size_t value_len = strlen(value);
                if (j + value_len >= initial_size) {
                    initial_size = j + value_len + 1;
                    char *new_result = ft_realloc(result, j, initial_size);
                    if (!new_result) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                ft_strlcpy(result + j, value, value_len + 1);
                j += value_len;
            }
        } else {
            if (j + 1 >= initial_size) {
                initial_size *= 2;
                char *new_result = ft_realloc(result, j, initial_size);
                if (!new_result) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[j++] = token[i++];
        }
    }
    result[j] = '\0';
    return result;
}
// char *expand_env_variables_in_token(char *token)
// {
//     if (!token)
//         return NULL;
//     size_t initial_size = ft_strlen(token) * 2 + 1;
//     char *result = ft_calloc(initial_size, sizeof(char));
//     if (!result)
//         return NULL;

//     int i = 0, j = 0;
//     while (token[i])
//     {
//         if (token[i] == '$')
//         {
//             i++;
//             if (token[i] == '\0' || isspace(token[i]))
//             {
//                 result[j++] = '$';
//                 continue;
//             }
//             char var_name[256];
//             int k = 0;
//             while (token[i] && (isalnum(token[i]) || token[i] == '_'))
//             {
//                 var_name[k++] = token[i++];
//             }
//             var_name[k] = '\0';
//             char *value = getenv(var_name);
//             if (value)
//             {
//                 size_t value_len = strlen(value);
//                 if (j + value_len >= initial_size)
//                 {
//                     initial_size = j + value_len + 1;
//                     result = realloc(result, initial_size * sizeof(char));
//                     if (!result)
//                         return NULL;
//                 }
//                 strcpy(result + j, value);
//                 j += value_len;
//             }
//         }
//         else
//         {
//             result[j++] = token[i++];
//         }
//     }
//     result[j] = '\0';
//     return result;
// }

t_cmd parse_cmd(char *input)
{
    t_parse_cmd parse_cmd;
    init_parse_cmd(&parse_cmd, input);
    while (parse_cmd.clean_input[parse_cmd.k])
    {
        if (parse_cmd.clean_input[parse_cmd.k] == '"' || parse_cmd.clean_input[parse_cmd.k] == '\'')
        {
            if (!parse_cmd.in_quotes)
            {
                parse_cmd.in_quotes = true;
                parse_cmd.quote_char = parse_cmd.clean_input[parse_cmd.k];
                parse_cmd.token_started = true;
                parse_cmd.k++;
                continue;
            }
            else if (parse_cmd.clean_input[parse_cmd.k] == parse_cmd.quote_char)
            {
                parse_cmd.in_quotes = false;
                parse_cmd.k++;
                continue;
            }
        }
        if (!parse_cmd.in_quotes && parse_cmd.clean_input[parse_cmd.k] == ' ')
        {
            if (parse_cmd.token_started)
            {
                parse_cmd.buffer[parse_cmd.j] = '\0';
                parse_cmd.cmd.args[parse_cmd.i++] = expand_env_variables_in_token(parse_cmd.buffer);
                if (parse_cmd.cmd.args[parse_cmd.i - 1] == NULL)
                {
                    free(parse_cmd.buffer);
                    free(parse_cmd.clean_input);
                    for (int i = 0; i < parse_cmd.cmd.arg_count; i++)
                        free(parse_cmd.cmd.args[i]);
                    free(parse_cmd.cmd.args);
                    parse_cmd.cmd.args = NULL;
                    parse_cmd.cmd.cmd = NULL;
                    parse_cmd.cmd.arg_count = 0;
                    return parse_cmd.cmd;
                }
                parse_cmd.j = 0;
                parse_cmd.token_started = false;
            }
            parse_cmd.k++;
            continue;
        }
        else
        {
            parse_cmd.buffer[parse_cmd.j++] = parse_cmd.clean_input[parse_cmd.k];
            parse_cmd.token_started = true;
        }
        parse_cmd.k++;
    }

    if (parse_cmd.token_started)
    {
        parse_cmd.buffer[parse_cmd.j] = '\0';
        parse_cmd.cmd.args[parse_cmd.i++] = expand_env_variables_in_token(parse_cmd.buffer);
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
    return (parse_cmd.cmd);
}
