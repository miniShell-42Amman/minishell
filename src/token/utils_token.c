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

#include "minishell.h"

int ft_is_operator(const char *str)
{
    if (*str == '|')
        return 1;
    if (*str == '>' || *str == '<')
    {
        if (*(str + 1) == *str)
            return 2;
        return 1;
    }
    return 0;
}
 
int count_args(char *input)
{
    int count = 0;
    int i = 0;
    int len = ft_strlen(input);

    while (i < len)
    {
        while (i < len && (input[i] == ' ' || input[i] == '\t'))
            i++;
        if (i >= len)
            break;
        if (input[i] == '|')
        {
            count++;
            i++;
            continue;
        }
        count++;
        int in_single = 0;
        int in_double = 0;
        while (i < len)
        {
            if (input[i] == '\'' && !in_double)
                in_single = !in_single;
            else if (input[i] == '"' && !in_single)
                in_double = !in_double;

            if (!in_single && !in_double)
            {
                if (input[i] == ' ' || input[i] == '\t' || input[i] == '|' || ft_is_operator(&input[i]))
                    break;
            }
            i++;
        }
    }
    return count;
}

// int count_args( char *input)
// {
//     int count = 0;
//     int in_single = 0, in_double = 0;
//     int i = 0;
//     int len = ft_strlen(input);

//     while (i < len)
//     {
//         while (i < len && (input[i] == ' ' || input[i] == '\t'))
//             i++;
//         if (i >= len)
//             break;
//         count++;
//         int word_start = i;
//         while (i < len)
//         {
//             if (input[i] == '\'' && !in_double)
//                 in_single = !in_single;
//             else if (input[i] == '"' && !in_single)
//                 in_double = !in_double;

//             if (!in_single && !in_double)
//                 if (input[i] == ' ' || input[i] == '\t' || ft_is_operator(&input[i]))
//                     break;
//             i++;
//         }
//         // if (i < len && input[i] == '|' && !in_single &&  !in_double)
//         //     count++;
//         i++;
//         if (i - word_start == 0 && (in_single || in_double))
//             count--;
//         ft_printf("count: %d\n", count);
//     }
//     return count;
// }


int *ft_count_token(char *input)
{ if (!input || input[0] == '\0')
        return NULL;
    if (input[0] == '|') 
        return NULL;
    int *array = NULL;
    int i = 0;
    int count = 0;
    while (input[i] != '\0')
    {
       if (input[i] == '|' || input[i] == '<' || input[i] == '>' || 
                     (input[i] == '<' && input[i + 1] == '<') || 
                     (input[i] == '>' && input[i + 1] == '>'))
            count++;
        i++;
    }
    if (count)
    {
        array = malloc(sizeof(int) * (count + 1));
        if (!array)
            return NULL;
    }
    count = 0;
    i = 0;
    int count_word = 0;
    int in_single_quotes = 0;
    int in_double_quotes = 0;

    while (input[i] == ' ' || input[i] == '\t')
        i++;
    while (input[i])
    {
        if (input[i] == '\'')
        {
            in_single_quotes = !in_single_quotes;
            i++;
        }
        else if (input[i] == '\"')
        {
            in_double_quotes = !in_double_quotes;
            i++;
        }
        else if ((input[i] != ' ' && input[i] != '\t') || in_single_quotes || in_double_quotes)
        {
            count_word++;
            while (((input[i] != ' ' && input[i] != '\t') || in_single_quotes || in_double_quotes) && input[i] != '\0')
            {
                if (input[i] == '\'')
                    in_single_quotes = !in_single_quotes;
                if (input[i] == '\"')
                    in_double_quotes = !in_double_quotes;
                if ((input[i] == '|' || input[i] == '<' || input[i] == '>' || 
                     (input[i] == '<' && input[i + 1] == '<') || 
                     (input[i] == '>' && input[i + 1] == '>')) && (in_single_quotes || in_double_quotes))
                        array[count++] = count_word - 1;    
                else if ((input[i] == '|' || input[i] == '<' || input[i] == '>' || 
                     (input[i] == '<' && input[i + 1] == '<') || 
                     (input[i] == '>' && input[i + 1] == '>')) && 
                    (input[i - 1] == '\'' || input[i - 1] == '\"' || 
                     input[i + 1] == '\'' || input[i + 1] == '\"'))
                {
                    array[count++] = count_word - 1;
                }
                i++;

            }
            if (count)
                array[count] = '\0';
        }
        else
            i++;
    }
    if (count == 0)
    {
        free(array);
        return NULL;
    }
    return (array);
}
