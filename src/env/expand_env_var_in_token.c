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

void	update_quote_state(char c, bool *squote, bool *dquote)
{
	if (c == '\'' && !*dquote)
		*squote = !*squote;
	else if (c == '"' && !*squote)
    {
		*dquote = !*dquote;
    }
}

size_t	handle_var_length(const char **token, t_env *env, size_t **len)
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
		if(**len)
		{
			var_len = **len;
			*token+=**len;
			**len = 0;
		}
		else 
		{
			while (**token && (ft_isalnum(**token) || **token == '_') && **token != '\'')
				(*token)++;
		}
		var_len = *token - start;
		value = get_var_value(env, start, var_len);
	}
	if (value)
		return (ft_strlen(value));
	return (0);
}

void	process_variable(const char **t, t_env *e, char **res, size_t *j, size_t **len)
{
	const char	*start;
	char		*value;
	size_t		var_len;
    var_len = 0;
	(*t)++;
	start = *t;
	if (**t == '?')
	{
		value = get_var_value(e, "?", 1);
		(*t)++;
	}
	else
	{
		if(**len)
		{
			var_len = **len;
			*t+=**len;
		}
		else 
		{
			while (**t && (ft_isalnum(**t) || **t == '_') && **t != '\'')
				(*t)++;
		}
		var_len = (size_t)(*t - start );
		value = get_var_value(e, start, var_len);
	}
	if (value)
	{
		var_len = 0;
		ft_strlcpy(*res + *j, value, ft_strlen(value) + 1);
		*j += ft_strlen(value);
	}

}

int is_dolloar_quote(const char *token)
{
	int i = 0;
	int count = 0;
	while (token[i])
	{
		if (token[i] == '$')
			count++;
		i++;
	}
	return (count);
}

char	*expand_env_variables_in_token(const char *token, t_env *env, size_t *len, t_parse_cmd *parse_cmd)
{
	char	*result;
	size_t	j;
	bool	squote;
	bool	dquote;
	if (!token || !env)
		return (ft_strdup(""));
	result = ft_calloc(calculate_length(token, env,&len, parse_cmd) + 1, sizeof(char));
	if (!result)
		return (NULL);
	j = 0;
	squote = false;
	dquote = false;
	(void)parse_cmd;

	if (token[0] == '\'' && token[ft_strlen(token) - 1] == '\'' && is_dolloar_quote(token) > 1 && token[1] != '"')
	{
		char *tmp = ft_substr(token, 1, ft_strlen(token) - 2);
		free((char *)token);
		token = ft_strdup(tmp);
		free(tmp);
	}
	while (*token)
	{
		update_quote_state(*token, &squote, &dquote);
		if (*token == '$'  && !squote)
		{	
			process_variable(&token, env, &result, &j, &len);
			continue ;
		}
		result[j++] = *token++;
	}
    result[j] = '\0';
	return (result);
}
