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

#include "minishell.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// نفترض وجود دوال ft_strlen, ft_calloc, ft_isspace, ft_isalnum و find_env_value.

// الدالة الأولى: تمريرة لحساب الحجم النهائي بعد التوسعة
static size_t compute_expanded_length(const char *token, t_env *env_list)
{
    size_t len = 0;
    size_t i = 0;
    bool in_single_quotes = false;
    bool in_double_quotes = false;

    while (token[i])
    {
        // إذا وجدنا اقتباس مفرد
        if (token[i] == '\'')
        {
            in_single_quotes = !in_single_quotes;
            len++; // نضيف علامة الاقتباس
            i++;
            continue;
        }
        // إذا وجدنا اقتباس مزدوج
        if (token[i] == '\"')
        {
            in_double_quotes = !in_double_quotes;
            len++;
            i++;
            continue;
        }
        // إذا وجدنا رمز الدولار للتوسعة وليس داخل اقتباس مفرد
        if (token[i] == '$' && !in_single_quotes)
        {
            i++; // نتخطى الرمز $
            // إذا كان الحرف التالي نهاية السلسلة أو فراغاً، نعتبر الـ $ حرفاً عاديًا
            if (token[i] == '\0' || ft_isspace(token[i]))
            {
                len++; 
                continue;
            }
            // استخراج اسم المتغير
            size_t start = i;
            while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
                i++;
            size_t var_len = i - start;
            if (var_len == 0)
            {
                len++; // لا يوجد اسم متغير صالح، نضيف $ حرفياً
            }
            else
            {
                char var_name[var_len + 1];
                ft_memcpy(var_name, token + start, var_len);
                var_name[var_len] = '\0';
                // البحث عن قيمة المتغير في قائمة البيئة
                char *value = find_env_value(env_list, var_name);
                if (value)
                {
                    len += ft_strlen(value);
                }
                // إذا لم يُعثر على المتغير، لا نضيف شيئًا (أي يُستبدل بـ string فارغ)
            }
            continue;
        }
        // حرف عادي يتم نسخه كما هو
        len++;
        i++;
    }
    return len;
}

// الدالة الرئيسية: توسعة المتغيرات البيئية داخل التوكن
char *expand_env_variables_in_token(const char *token, t_env *env_list)
{
    if (!token)
        return NULL;

    // المرور الأول: حساب الحجم النهائي المطلوب بعد التوسعة
    size_t final_length = compute_expanded_length(token, env_list);
    // تخصيص الذاكرة مرة واحدة بالحجم النهائي + 1 للمحرف '\0'
    char *result = ft_calloc(final_length + 1, sizeof(char));
    if (!result)
        return NULL;

    size_t i = 0, j = 0;
    bool in_single_quotes = false;
    bool in_double_quotes = false;

    // المرور الثاني: بناء السلسلة الموسعة
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
            i++; // نتخطى رمز $
            if (token[i] == '\0' || ft_isspace(token[i]))
            {
                result[j++] = '$';
                continue;
            }
            size_t start = i;
            while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
                i++;
            size_t var_len = i - start;
            if (var_len == 0)
            {
                result[j++] = '$';
            }
            else
            {
                char var_name[var_len + 1];
                ft_memcpy(var_name, token + start, var_len);
                var_name[var_len] = '\0';
                char *value = find_env_value(env_list, var_name);
                if (value)
                {
                    size_t value_len = ft_strlen(value);
                    ft_memcpy(result + j, value, value_len);
                    j += value_len;
                }
                // إذا لم يوجد المتغير، نستبدله بسلسلة فارغة (لا نضيف شيئًا)
            }
            continue;
        }
        result[j++] = token[i++];
    }
    result[j] = '\0';
    return result;
}
