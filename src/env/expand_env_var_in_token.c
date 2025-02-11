/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var_in_token.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:49:12 by oissa             #+#    #+#             */
/*   Updated: 2025/02/05 19:02:19 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_env_variables_in_token(char *token, t_env *env_list)
{
    if (token == NULL)
        return NULL;
    size_t initial_size = ft_strlen(token) * 2 + 1;
    char *result = ft_calloc(initial_size, sizeof(char));
    if (!result)
        return NULL;

    size_t i = 0;
    size_t j = 0;
    bool in_single_quotes = false;
    bool in_double_quotes = false;

    while (token[i])
    {
        if (token[i] == '\'')
        {
            in_single_quotes = !in_single_quotes;
            result[j++] = token[i++];
            continue;
        }
        if (token[i] == '\"')
        {
            in_double_quotes = !in_double_quotes;
            result[j++] = token[i++];
            continue;
        }

        if (token[i] == '$' && !in_single_quotes)
        {
            i++;
            if (token[i] == '\0' || ft_isspace(token[i]))
            {
                result[j++] = '$';
                continue;
            }
            size_t name_buffer_size = 64;
            char *var_name = malloc(name_buffer_size);
            if (!var_name)
            {
                free(result);
                return NULL;
            }
            size_t k = 0;
            while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
            {
                if (k + 1 >= name_buffer_size)
                {
                    name_buffer_size *= 2;
                    char *temp = ft_realloc(var_name, k, name_buffer_size);
                    if (!temp)
                    {
                        free(var_name);
                        free(result);
                        return NULL;
                    }
                    var_name = temp;
                }
                var_name[k++] = token[i++];
            }
            var_name[k] = '\0';

            char *value = find_env_value(env_list, var_name);
            free(var_name);

            if (value)
            {
                size_t value_len = ft_strlen(value);
                if (j + value_len >= initial_size)
                {
                    initial_size = j + value_len + 1;
                    char *new_result = ft_realloc(result, j, initial_size);
                    if (!new_result)
                    {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                ft_strlcpy(result + j, value, value_len + 1);
                j += value_len;
            }
        }
        else
        {
            if (j + 1 >= initial_size)
            {
                initial_size *= 2;
                char *new_result = ft_realloc(result, j, initial_size);
                if (!new_result)
                {
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
