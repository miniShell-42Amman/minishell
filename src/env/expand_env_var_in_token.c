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

// char	*expand_env_variables_in_token(char *token, t_env *env_list)
// {
// 	size_t	initial_size;
// 	char	*result;
// 	size_t	i;
// 	size_t	j;
// 	bool	in_single_quotes;
// 	bool	in_double_quotes;
// 	size_t	name_buffer_size;
// 	char	*var_name;
// 	size_t	k;
// 	char	*temp;
// 	char	*value;
// 	size_t	value_len;
// 	char	*new_result;

// 	if (token == NULL)
// 		return (NULL);
// 	initial_size = ft_strlen(token) * 2 + 1;
// 	result = ft_calloc(initial_size, sizeof(char));
// 	if (!result)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	in_single_quotes = false;
// 	in_double_quotes = false;
// 	while (token[i])
// 	{
// 		if (token[i] == '\'')
// 		{
// 			in_single_quotes = !in_single_quotes;
// 			result[j++] = token[i++];
// 			continue ;
// 		}
// 		if (token[i] == '\"')
// 		{
// 			in_double_quotes = !in_double_quotes;
// 			result[j++] = token[i++];
// 			continue ;
// 		}
// 		if (token[i] == '$' && !in_single_quotes)
// 		{
// 			i++;
// 			if (token[i] == '\0' || ft_isspace(token[i]))
// 			{
// 				result[j++] = '$';
// 				continue ;
// 			}
// 			name_buffer_size = 64;
// 			var_name = malloc(name_buffer_size);
// 			if (!var_name)
// 			{
// 				free(result);
// 				return (NULL);
// 			}
// 			k = 0;
// 			while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
// 			{
// 				if (k + 1 >= name_buffer_size)
// 				{
// 					name_buffer_size *= 2;
// 					temp = ft_realloc(var_name, k, name_buffer_size);
// 					if (!temp)
// 					{
// 						free(var_name);
// 						free(result);
// 						return (NULL);
// 					}
// 					var_name = temp;
// 				}
// 				var_name[k++] = token[i++];
// 			}
// 			var_name[k] = '\0';
// 			value = find_env_value(env_list, var_name);
// 			free(var_name);
// 			if (value)
// 			{
// 				value_len = ft_strlen(value);
// 				if (j + value_len >= initial_size)
// 				{
// 					initial_size = j + value_len + 1;
// 					new_result = ft_realloc(result, j, initial_size);
// 					if (!new_result)
// 					{
// 						free(result);
// 						return (NULL);
// 					}
// 					result = new_result;
// 				}
// 				ft_strlcpy(result + j, value, value_len + 1);
// 				j += value_len;
// 			}
// 		}
// 		else
// 		{
// 			if (j + 1 >= initial_size)
// 			{
// 				initial_size *= 2;
// 				new_result = ft_realloc(result, j, initial_size);
// 				if (!new_result)
// 				{
// 					free(result);
// 					return (NULL);
// 				}
// 				result = new_result;
// 			}
// 			result[j++] = token[i++];
// 		}
// 	}
// 	result[j] = '\0';
// 	return (result);
// }
typedef struct s_expand_env_var_in_token
{
    size_t len;
    size_t i;
    size_t j;
    size_t final_length;
    size_t var_len;
    size_t value_len;
    size_t start;
    bool in_single_quotes;
    bool in_double_quotes;
    char *result;
    char *value;
    char *var_name;
    
} t_expand_env_var_in_token;

static size_t compute_expanded_length(const char *token, t_env *env_list)
{
    size_t len = 0;
    size_t i = 0;
    bool in_single_quotes = false;
    bool in_double_quotes = false;

    while (token[i])
    {
        if (token[i] == '\'')
        {
            in_single_quotes = !in_single_quotes;
            len++;
            i++;
            continue;
        }
        if (token[i] == '\"')
        {
            in_double_quotes = !in_double_quotes;
            len++;
            i++;
            continue;
        }
        if (token[i] == '$' && !in_single_quotes)
        {
            i++;
            if (token[i] == '\0' || ft_isspace(token[i]))
            {
                len++; 
                continue;
            }
            size_t start = i;
            while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
                i++;
            size_t var_len = i - start;
            if (var_len == 0)
                len++;
            else
            {
                char var_name[var_len + 1];
                ft_memcpy(var_name, token + start, var_len);
                var_name[var_len] = '\0';
                char *value = find_env_value(env_list, var_name);
                if (value)
                    len += ft_strlen(value);
            }
            continue;
        }
        len++;
        i++;
    }
    return len;
}

char *expand_env_variables_in_token(const char *token, t_env *env_list)
{
    if (!token)
        return NULL;
    
    t_expand_env_var_in_token t_expand;

    ft_bzero(&t_expand, sizeof(t_expand));

    t_expand.final_length = compute_expanded_length(token, env_list);
    t_expand.result = ft_calloc(t_expand.final_length + 1, sizeof(char));
    if (!t_expand.result)
        return NULL;


    while (token[t_expand.i])
    {
        if (token[t_expand.i] == '\'')
        {
            t_expand.in_single_quotes = !t_expand.in_single_quotes;
            t_expand.result[t_expand.j++] = token[t_expand.i++];
            continue;
        }
        if (token[t_expand.i] == '\"')
        {
            t_expand.in_double_quotes = !t_expand.in_double_quotes;
            t_expand.result[t_expand.j++] = token[t_expand.i++];
            continue;
        }
        if (token[t_expand.i] == '$' && !t_expand.in_single_quotes)
        {
            t_expand.i++;
            if (token[t_expand.i] == '\0' || ft_isspace(token[t_expand.i]))
            {
                t_expand.result[t_expand.j++] = '$';
                continue;
            }
            t_expand.start = t_expand.i;
            while (token[t_expand.i] && (ft_isalnum(token[t_expand.i]) || token[t_expand.i] == '_'))
            t_expand.i++;
            t_expand.var_len = t_expand.i - t_expand.start;
            if (t_expand.var_len == 0)
            t_expand.result[t_expand.j++] = '$';
            else
            {
                t_expand.var_name = malloc(t_expand.var_len + 1);
                if (!t_expand.var_name)
                {
                    free(t_expand.result);
                    return NULL;
                }
                ft_memcpy(t_expand.var_name, token + t_expand.start, t_expand.var_len);
                t_expand.var_name[t_expand.var_len] = '\0';
                t_expand.value = find_env_value(env_list, t_expand.var_name);
                free(t_expand.var_name);
                if (t_expand.value)
                {
                    t_expand.value_len = ft_strlen(t_expand.value);
                    ft_memcpy(t_expand.result + t_expand.j, t_expand.value, t_expand.value_len);
                    t_expand.j += t_expand.value_len;
                }
            }
            continue;
        }
        t_expand.result[t_expand.j++] = token[t_expand.i++];
    }
    t_expand.result[t_expand.j] = '\0';
    return t_expand.result;
}
