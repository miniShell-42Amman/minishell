/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 00:25:22 by oissa             #+#    #+#             */
/*   Updated: 2025/02/02 00:25:22 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

    int i = 0;
    while (input[i])
    {
        if (input[i] == '"' || input[i] == '\'')
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
        i++;
    }
    return count;
}
