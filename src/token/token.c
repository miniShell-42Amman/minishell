/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 18:18:01 by oissa             #+#    #+#             */
/*   Updated: 2025/02/11 18:18:01 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int init_parse_cmd(t_parse_cmd *parse_cmd, char *input)
{
    ft_bzero(parse_cmd, sizeof(t_parse_cmd));
    ft_bzero(&parse_cmd->cmd, sizeof(t_cmd));
    parse_cmd->trimmed_input = ft_strtrim(input, " \t\n");
    parse_cmd->clean_input = ft_strdup(parse_cmd->trimmed_input);
    free(parse_cmd->trimmed_input);
    if (!parse_cmd->clean_input)
        return (EXIT_FAILURE);
    parse_cmd->buffer = ft_calloc((ft_strlen(parse_cmd->clean_input) + 1), sizeof(char));
    if (!parse_cmd->buffer)
    {
        free(parse_cmd->clean_input);
        return (EXIT_FAILURE);
    }
    parse_cmd->cmd.arg_count = count_args(parse_cmd->clean_input);
    parse_cmd->cmd.args = ft_calloc((parse_cmd->cmd.arg_count + 2), sizeof(char *));
    if (!parse_cmd->cmd.args)
    {
        free(parse_cmd->clean_input);
        free(parse_cmd->buffer);
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

static t_cmd ft_free_parse_cmd(t_parse_cmd *parse_cmd)
{
    int i;
    
    ft_printf(RED "Error: " RESET "Unclosed quotes\n");
    if (parse_cmd->clean_input)
        free(parse_cmd->clean_input);
    i = 0;
    while (i < parse_cmd->cmd.arg_count)
    {
        free(parse_cmd->cmd.args[i]);
        i++;
    }
    if (parse_cmd->cmd.args)
        free(parse_cmd->cmd.args);
    if (parse_cmd->buffer)
        free(parse_cmd->buffer);
    parse_cmd->cmd.args = NULL;
    parse_cmd->cmd.cmd = NULL;
    parse_cmd->cmd.arg_count = 0;
    return (parse_cmd->cmd);
}
static int clean_parse_cmd(t_parse_cmd *parse_cmd)
{
    parse_cmd->cmd.args[parse_cmd->i] = NULL;
    if (parse_cmd->cmd.args[0])
    {
        parse_cmd->cmd.cmd = ft_strdup(parse_cmd->cmd.args[0]);
        if (parse_cmd->cmd.cmd == NULL)
            return (EXIT_FAILURE);
    }
    else
        parse_cmd->cmd.cmd = NULL;
    if (parse_cmd->clean_input)
        free(parse_cmd->clean_input);
    if (parse_cmd->buffer)
        free(parse_cmd->buffer);
    return (EXIT_SUCCESS);
}

static int if_token_started(t_parse_cmd *parse_cmd, t_env *env_list)
{
    if (parse_cmd->token_started)
    {
        parse_cmd->buffer[parse_cmd->j] = '\0';
        if (parse_cmd->token_was_single_quoted || parse_cmd->token_was_dollar_quote)
        {
            parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup(parse_cmd->buffer);
            if (parse_cmd->cmd.args[parse_cmd->i - 1] == NULL)
                return (EXIT_FAILURE);
        }
        else
            parse_cmd->cmd.args[parse_cmd->i++] = expand_env_variables_in_token(parse_cmd->buffer, env_list);
        parse_cmd->j = 0;
        parse_cmd->token_started = false;
        parse_cmd->token_quote_type = '\0';
        parse_cmd->token_was_single_quoted = false;
        parse_cmd->token_was_dollar_quote = false;
    }
    return (EXIT_SUCCESS);
}

static int if_token_started_too(t_parse_cmd *parse_cmd, t_env *env_list)
{
    if (parse_cmd->token_started)
    {
        parse_cmd->buffer[parse_cmd->j] = '\0';
        if (parse_cmd->token_was_single_quoted || parse_cmd->token_was_dollar_quote)
        {
            parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup(parse_cmd->buffer);
            if (parse_cmd->cmd.args[parse_cmd->i - 1] == NULL)
            {
                ft_bzero(&parse_cmd->cmd, sizeof(t_cmd));
                return (EXIT_FAILURE);
            }
        }
        else
            parse_cmd->cmd.args[parse_cmd->i++] = expand_env_variables_in_token(parse_cmd->buffer, env_list);
    }
    return (EXIT_SUCCESS);
}

static int ft_check_parse_cmd(t_parse_cmd *parse_cmd)
{
    if ((parse_cmd->c == '\'' || parse_cmd->c == '"') && parse_cmd->token_quote_type == '\0')
    {
        parse_cmd->token_quote_type = parse_cmd->c;
        parse_cmd->in_quotes = true;
        parse_cmd->token_started = true;
        if (parse_cmd->c == '\'')
            parse_cmd->token_was_single_quoted = true;
        parse_cmd->k++;
        return (EXIT_SUCCESS);
    }
    else if (parse_cmd->c == parse_cmd->token_quote_type && parse_cmd->in_quotes)
    {
        parse_cmd->in_quotes = false;
        parse_cmd->token_quote_type = '\0';
        parse_cmd->k++;
        return (EXIT_SUCCESS);
    }
    return (EXIT_FAILURE);
}

t_cmd parse_cmd(char *input, t_env *env_list)
{
    t_parse_cmd parse_cmd;
    if (init_parse_cmd(&parse_cmd, input) == EXIT_FAILURE)
        return (parse_cmd.cmd);
    // parse_cmd.token_quote_type = '\0';

    while (parse_cmd.clean_input[parse_cmd.k])
    {
        parse_cmd.c = parse_cmd.clean_input[parse_cmd.k];

        if (!parse_cmd.token_started && parse_cmd.c == '$' && parse_cmd.clean_input[parse_cmd.k + 1] == '"')
        {
            parse_cmd.token_was_dollar_quote = true;
            parse_cmd.k++;
            continue;
        }
        if (ft_check_parse_cmd(&parse_cmd) == EXIT_SUCCESS)
            continue;
        if (!parse_cmd.in_quotes && (parse_cmd.c == ' ' || parse_cmd.c == '|'))
        {
            if (if_token_started(&parse_cmd, env_list) == EXIT_FAILURE)
            {
                ft_bzero(&parse_cmd.cmd, sizeof(t_cmd));
                return (parse_cmd.cmd);
            }    
            if (parse_cmd.c == '|')
            {
                parse_cmd.cmd.args[parse_cmd.i++] = ft_strdup("|");
                if (parse_cmd.cmd.args[parse_cmd.i - 1] == NULL)
                {
                    ft_bzero(&parse_cmd.cmd, sizeof(t_cmd));
                    return (parse_cmd.cmd);
                }
            }
            parse_cmd.k++;
            continue;
        }
        else
        {
            parse_cmd.buffer[parse_cmd.j++] = parse_cmd.c;
            parse_cmd.token_started = true;
        }
        parse_cmd.k++;
        
    }
    if (if_token_started_too(&parse_cmd, env_list) == EXIT_FAILURE)
        return (parse_cmd.cmd);
    if (parse_cmd.in_quotes)
        return (ft_free_parse_cmd(&parse_cmd));
    if (clean_parse_cmd(&parse_cmd) == EXIT_FAILURE)
    {
        ft_bzero(&parse_cmd.cmd, sizeof(t_cmd));
        return (parse_cmd.cmd);
    }
    return (parse_cmd.cmd);
}
