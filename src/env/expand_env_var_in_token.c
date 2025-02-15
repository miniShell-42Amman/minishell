/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var_in_token.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 12:00:00 by lalhindi          #+#    #+#             */
/*   Updated: 2023/10/01 12:00:00 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	calculate_length(const char *token, t_env *env);
static void		update_quote_state(char c, bool *squote, bool *dquote);
static size_t	handle_var_length(const char **token, t_env *env);
static void		process_variable(const char **t, t_env *e, char **res, size_t *j);
static char		*get_var_value(t_env *env, const char *var, size_t len);

char	*expand_env_variables_in_token(const char *token, t_env *env)
{
	char	*result;
	size_t	j;
	bool	squote;
	bool	dquote;

	if (!token || !env)
		return (ft_strdup(""));
	result = ft_calloc(calculate_length(token, env) + 1, sizeof(char));
	if (!result)
		return (NULL);
	j = 0;
	squote = false;
	dquote = false;
	while (*token)
	{
		update_quote_state(*token, &squote, &dquote);
		if (*token == '$' && !squote)
		{
			process_variable(&token, env, &result, &j);
			continue ;
		}
		result[j++] = *token++;
	}
	return (result);
}

static size_t	calculate_length(const char *token, t_env *env)
{
	size_t	len;
	bool	squote;
	bool	dquote;

	len = 0;
	squote = false;
	dquote = false;
	while (*token)
	{
		update_quote_state(*token, &squote, &dquote);
		if (*token == '$' && !squote)
		{
			token++;
			len += handle_var_length(&token, env);
			continue ;
		}
		len++;
		token++;
	}
	return (len);
}

static void	update_quote_state(char c, bool *squote, bool *dquote)
{
	if (c == '\'' && !*dquote)
		*squote = !*squote;
	else if (c == '"' && !*squote)
		*dquote = !*dquote;
}

static size_t	handle_var_length(const char **token, t_env *env)
{
	const char	*start;
	char		*value;
	size_t		var_len;

	start = *token;
	if (**token == '?')
	{
		(*token)++;
		value = get_var_value(env, "?", 1);
	}
	else
	{
		while (**token && (ft_isalnum(**token) || **token == '_'))
			(*token)++;
		var_len = *token - start;
		value = get_var_value(env, start, var_len);
	}
	if (value)
		return (ft_strlen(value));
	return (0);
}

static void	process_variable(const char **t, t_env *e, char **res, size_t *j)
{
	const char	*start;
	char		*value;
	size_t		var_len;

	(*t)++;
	start = *t;
	if (**t == '?')
	{
		value = get_var_value(e, "?", 1);
		(*t)++;
	}
	else
	{
		while (**t && (ft_isalnum(**t) || **t == '_'))
			(*t)++;
		var_len = *t - start;
		value = get_var_value(e, start, var_len);
	}
	if (value)
	{
		ft_strlcpy(*res + *j, value, ft_strlen(value) + 1);
		*j += ft_strlen(value);
	}
}

static char	*get_var_value(t_env *env, const char *var, size_t len)
{
	char	var_name[len + 1];
	size_t	i;

	i = 0;
	while (i < len)
	{
		var_name[i] = var[i];
		i++;
	}
	var_name[i] = '\0';
	while (env)
	{
		if (ft_strcmp(env->key, var_name) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

// typedef struct s_expand_env_var_in_token
// {
//     size_t len;
//     size_t i;
//     size_t j;
//     size_t final_length;
//     size_t var_len;
//     size_t value_len;
//     size_t start;
//     bool in_single_quotes;
//     bool in_double_quotes;
//     char *result;
//     char *value;
//     char *var_name;
    
// } t_expand_env_var_in_token;

// static size_t compute_expanded_length(const char *token, t_env *env_list)
// {
//     size_t len = 0;
//     size_t i = 0;
//     bool in_single_quotes = false;
//     bool in_double_quotes = false;

//     while (token[i])
//     {
//         if (token[i] == '\'')
//         {
//             in_single_quotes = !in_single_quotes;
//             len++;
//             i++;
//             continue;
//         }
//         if (token[i] == '\"')
//         {
//             in_double_quotes = !in_double_quotes;
//             len++;
//             i++;
//             continue;
//         }
//         if (token[i] == '$' && !in_single_quotes)
//         {
//             i++;
//             if (token[i] == '\0' || ft_isspace(token[i]))
//             {
//                 len++; 
//                 continue;
//             }
//             size_t start = i;
//             while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
//                 i++;
//             size_t var_len = i - start;
//             if (var_len == 0)
//                 len++;
//             else
//             {
//                 char var_name[var_len + 1];
//                 ft_memcpy(var_name, token + start, var_len);
//                 var_name[var_len] = '\0';
//                 char *value = find_env_value(env_list, var_name);
//                 if (value)
//                     len += ft_strlen(value);
//             }
//             continue;
//         }
//         len++;
//         i++;
//     }
//     return len;
// }

// char *expand_env_variables_in_token(const char *token, t_env *env_list)
// {
//     if (!token)
//         return NULL;
    
//     t_expand_env_var_in_token t_expand;
//     ft_printf("%s\n", token);
//     ft_bzero(&t_expand, sizeof(t_expand));

//     t_expand.final_length = compute_expanded_length(token, env_list);
//     ft_printf("final_length: %d\n", t_expand.final_length);
//     ft_printf("token: %s\n", env_list->value);
//     t_expand.result = ft_calloc(t_expand.final_length + 1, sizeof(char));
//     if (!t_expand.result)
//         return NULL;

//     while (token[t_expand.i])
//     {
//         if (token[t_expand.i] == '\'')
//         {
//             t_expand.in_single_quotes = !t_expand.in_single_quotes;
//             t_expand.result[t_expand.j++] = token[t_expand.i++];
//             continue;
//         }
//         if (token[t_expand.i] == '\"')
//         {
//             t_expand.in_double_quotes = !t_expand.in_double_quotes;
//             t_expand.result[t_expand.j++] = token[t_expand.i++];
//             continue;
//         }
//         if (token[t_expand.i] == '$' && !t_expand.in_single_quotes)
//         {
//             t_expand.i++;
//             if (token[t_expand.i] == '\0' || ft_isspace(token[t_expand.i]))
//             {
//                 t_expand.result[t_expand.j++] = '$';
//                 continue;
//             }
//             t_expand.start = t_expand.i;
//             while (token[t_expand.i] && (ft_isalnum(token[t_expand.i]) || token[t_expand.i] == '_'))
//                 t_expand.i++;
//             t_expand.var_len = t_expand.i - t_expand.start;
//             if (t_expand.var_len == 0)
//                 t_expand.result[t_expand.j++] = '$';
//             else
//             {
//                 t_expand.var_name = malloc(t_expand.var_len + 1);
//                 if (!t_expand.var_name)
//                 {
//                     free(t_expand.result);
//                     return NULL;
//                 }
//                 ft_memcpy(t_expand.var_name, token + t_expand.start, t_expand.var_len);
//                 t_expand.var_name[t_expand.var_len] = '\0';
//                 t_expand.value = find_env_value(env_list, t_expand.var_name);
//                 free(t_expand.var_name);
//                 if (t_expand.value)
//                 {
//                     t_expand.value_len = ft_strlen(t_expand.value);
//                     ft_memcpy(t_expand.result + t_expand.j, t_expand.value, t_expand.value_len);
//                     t_expand.j += t_expand.value_len;
//                 }
//             }
//             continue;
//         }
//         t_expand.result[t_expand.j++] = token[t_expand.i++];
//     }
//     t_expand.result[t_expand.j] = '\0';
//     return t_expand.result;
// }
