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
#include <stdbool.h>

void	update_quote_state(char c, bool *squote, bool *dquote)
{
	if (c == '\'' && !*dquote)
		*squote = !*squote;
	else if (c == '"' && !*squote)
    {
		*dquote = !*dquote;
    }
}

size_t	handle_var_length(const char **token, t_env *env, t_parse_cmd *p)
{
	const char	*start;
	char		*value;
	size_t		var_len;
	// ft_printf("p arr has dollar count = %zu\n", p->arr_has_dollar_count);
	var_len = 0;
	start = *token;
	if (**token == '?')
	{
		value = get_var_value(env, "?", 1);
		(*token)++;
	}
	else
	{
		if(p->arr_has_dollar && p->arr_has_dollar[p->arr_has_dollar_count] != (size_t)-1)
		{
			var_len = p->arr_has_dollar[p->arr_has_dollar_count];
			*token+=var_len;
			p->arr_has_dollar_count++;
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

void	process_variable(const char **t, t_env *e, char **res, size_t *j, t_parse_cmd *p)
{
	const char	*start;
	char		*value;
	size_t		var_len;
	// ft_printf("p arr has dollar count = %zu\n", p->arr_has_dollar_count);
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
		if(p->arr_has_dollar && p->arr_has_dollar[p->arr_has_dollar_count] != (size_t)-1)
		{
			var_len = p->arr_has_dollar[p->arr_has_dollar_count];
			*t+=var_len;
			p->arr_has_dollar_count++;
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
	while (token[i] && token[i] != ' ')
	{
		if ( token[i] == '$')
			count++;
		i++;
	}
	return (count);
}
void calculate_dollar_array(t_parse_cmd *p)
{
	int i = 0;
	int j = 0;
	while (p->splitter_clean_input[p->index_splitter][i])
	{
		if (p->splitter_clean_input[p->index_splitter][i] == '$')
		{
			j++;
		}
		i++;
	}
	p->arr_has_dollar = malloc(sizeof(size_t) * (j + 1));
	if (!p->arr_has_dollar)
		return ;
	i = 0;
	j = 0;
	size_t help = 0;
	while (p->splitter_clean_input[p->index_splitter][i])
	{
		if (p->splitter_clean_input[p->index_splitter][i] == '$')
		{
			help = 0;
			i++;
			while (p->splitter_clean_input[p->index_splitter][i + help] && (ft_isalnum(p->splitter_clean_input[p->index_splitter][i + help]) || p->splitter_clean_input[p->index_splitter][i + help] == '_'))
			{
				help++;
			}
			p->arr_has_dollar[j++] = help;
		}
		else
		{
			i++;
		}
	}
	p->arr_has_dollar[j] = (size_t)-1;
}

char	*expand_env_variables_in_token(const char *token, t_env *env, t_parse_cmd *parse_cmd)
{
	char	*result;
	size_t	j;
	bool	squote;
	bool	dquote;

	j= 0;
	if (!token || !env)
		return (ft_strdup(""));
	// ft_printf("how are you\n");
	if (token[0] == '\'' && token[ft_strlen(token) - 1] == '\'' && token[1] != '"')
	{
		result = ft_calloc(calculate_length(token, env, parse_cmd) + 1, sizeof(char));
		ft_printf("result = %s\n", token);
		result[0] = '\'';
		char *tmp = ft_substr(token, 1, ft_strlen(token) - 2);
		free((char *)token);
		token = ft_strdup(tmp);
		free(tmp);
		result[ft_strlen(token) + 1] = '\'';
		return (result);
	}
	parse_cmd->arr_has_dollar_count = 0;
	result = ft_calloc(calculate_length(token, env, parse_cmd) + 1, sizeof(char));
	parse_cmd->arr_has_dollar_count = 0;
	
	if(parse_cmd->splitter_clean_input[parse_cmd->index_splitter] &&  is_dolloar_quote(token) == is_dolloar_quote(parse_cmd->splitter_clean_input[parse_cmd->index_splitter]) &&  is_dolloar_quote(token) > 0 )
		calculate_dollar_array(parse_cmd);
	while(parse_cmd->arr_has_dollar && parse_cmd->arr_has_dollar[parse_cmd->arr_has_dollar_count] != (size_t)-1)
	{
		parse_cmd->arr_has_dollar_count++;
	}
	if (!result)
		return (NULL);
	j = 0;
	squote = false;
	dquote = false;
	while (*token)
	{
		update_quote_state(*token, &squote, &dquote);
		if (*token == '$')
		{	
			process_variable(&token, env, &result, &j, parse_cmd);
			continue ;
		}
		result[j++] = *token++;
	}
	parse_cmd->arr_has_dollar_count = 0;
	// free(parse_cmd->arr_has_dollar);
	// parse_cmd->arr_has_dollar = NULL;
	if (parse_cmd->arr_has_dollar) {
        free(parse_cmd->arr_has_dollar);
        parse_cmd->arr_has_dollar = NULL;
    }
    result[j] = '\0';
	return (result);
}
