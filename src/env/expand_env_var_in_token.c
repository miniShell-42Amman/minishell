#include "minishell.h"

void	update_quote_state(char c, bool *squote, bool *dquote)
{
	if (c == '\'' && !*dquote)
		*squote = !*squote;
	else if (c == '"' && !*squote)
		*dquote = !*dquote;
}

size_t	handle_var_length(const char **token, t_env *env, t_parse_cmd *p)
{
	const char	*start;
	char		*value;
	size_t		var_len;
	int			should_free;
		char var_name[2] = {**token, '\0'};
	size_t		len;

	value = NULL;
	should_free = 0;
	var_len = 0;
	start = *token;
	if (**token == '?')
	{
		value = ft_itoa(*p->exit_status);
		should_free = 1;
		(*token)++;
	}
	else if (**token == '0')
	{
		value = ft_strdup(p->program_name);
		should_free = 1;
		(*token)++;
	}
	else if (ft_isdigit(**token))
	{
		value = get_var_value(env, var_name, 1);
		should_free = 1;
		(*token)++;
	}
	else
	{
		if (p->arr_has_dollar
			&& p->arr_has_dollar[p->arr_has_dollar_count] != (size_t)-1)
		{
			var_len = p->arr_has_dollar[p->arr_has_dollar_count];
			*token += var_len;
			p->arr_has_dollar_count++;
		}
		else
		{
			while (**token && (ft_isalnum(**token) || **token == '_')
				&& **token != '\'')
				(*token)++;
		}
		var_len = *token - start;
		value = get_var_value(env, start, var_len);
		should_free = 0;
	}
	if (value)
	{
		len = ft_strlen(value);
		if (should_free)
			free(value);
		return (len);
	}
	return (0);
}

void	process_variable(t_expand_env *exen)
{
	const char	*start;
	char		*value;
	size_t		var_len;
	int			should_free;
	char var_name[2] = {*exen->token, '\0'};

	value = NULL;
	should_free = 0;
	var_len = 0;
	// (*t)++;
	exen->token++;
	start = exen->token;
	if (*exen->token == '?')
	{
		value = ft_itoa(exen->parse_cmd->exit_status);
		should_free = 1;
		// (*t)++;
		exen->token++;
	}
	else if (*exen->token == '0')
	{
		value = ft_strdup(exen->parse_cmd->program_name);
		should_free = 1;
		// (*t)++;
		exen->token++;
	}
	else if (ft_isdigit(*exen->token))
	{
		value = get_var_value(exen->env, var_name, 1);
		should_free = 1;
		// (*t)++;
		exen->token++;
	}
	else if (*exen->token == '_')
	{
		value = get_var_value(exen->env, "_", 1);
		should_free = 0;
		// (*t)++;
		exen->token++;
	}
	else if (exen->parse_cmd->arr_has_dollar
		&& exen->parse_cmd->arr_has_dollar[exen->parse_cmd->arr_has_dollar_count] != (size_t)-1)
	{
		var_len = exen->parse_cmd->arr_has_dollar[exen->p->arr_has_dollar_count];
		*exen->token += var_len;
		p->arr_has_dollar_count++;
		value = get_var_value(exen->env, start, var_len);
		if (value && *value && ft_strchr(value, ' '))
			p->must_splitter[exen->parse_cmd->index_splitter] = (size_t)1;
		else
			p->must_splitter[exen->parse_cmd->index_splitter] = (size_t)0;
		should_free = 0;
	}
	else
	{
		while (**t && (ft_isalnum(**t) || **t == '_') && **t != '\'')
			(*t)++;
		var_len = (size_t)(*t - start);
		value = get_var_value(e, start, var_len);
		if (*value && ft_strchr(value, ' '))
			p->must_splitter[p->index_splitter] = (size_t)1;
		else
			p->must_splitter[p->index_splitter] = (size_t)0;
		should_free = 0;
	}
	if (value)
	{
		ft_strlcpy(*res + *j, value, ft_strlen(value) + 1);
		*j += ft_strlen(value);
		if (should_free)
			free(value);
	}
}

int	is_dolloar_quote(const char *token)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!token || !*token)
		return (0);
	while (token[i] && token[i] != ' ')
	{
		if (token[i] == '$')
			count++;
		i++;
	}
	return (count);
}

void	calculate_dollar_array(t_parse_cmd *p)
{
	int		i;
	int		j;
	size_t	help;

	i = 0;
	j = 0;
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
	help = 0;
	while (p->splitter_clean_input[p->index_splitter][i])
	{
		if (p->splitter_clean_input[p->index_splitter][i] == '$')
		{
			if (p->splitter_clean_input[p->index_splitter][i + 1] == '?')
			{
				p->arr_has_dollar[j++] = 1;
				i += 2;
				continue ;
			}
			help = 0;
			i++;
			while (p->splitter_clean_input[p->index_splitter][i + help]
				&& (ft_isalnum(p->splitter_clean_input[p->index_splitter][i
						+ help]) || p->splitter_clean_input[p->index_splitter][i
					+ help] == '_'))
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
int	is_string_inside_single(const char *token)
{
	int		i;
	int		quote_count;
	char	parent;

	i = 0;
	quote_count = 0;
	parent = 0;
	if (!token)
		return (0);
	while (token[i])
	{
		if (token[i] == '\'' || token[i] == '"')
		{
			if (parent == 0)
				parent = token[i];
			else if (parent == token[i])
				parent = 0;
			quote_count++;
		}
		i++;
	}
	if (quote_count % 2 != 0)
		return (0);
	if (parent == '\'')
		return (1);
	else
		return (0);
}
int	ft_arr_len(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}




void expand_loop(t_expand_env *expand_env)
{
	while (*expand_env->token)
	{
		update_quote_state(*expand_env->token, &expand_env->squote, &expand_env->dquote);
		if (*expand_env->token == '$'
			&& !is_string_inside_single(expand_env->parse_cmd->splitter_clean_input[expand_env->parse_cmd->index_splitter])
			&& (*(expand_env->token + 1) != ' '
			&& *(expand_env->token + 1) != '\0'))
		{
			if (expand_env->parse_cmd->index_splitter >= 1
				&& ft_strnstr(expand_env->parse_cmd->splitter_clean_input
					[expand_env->parse_cmd->index_splitter - 1], "<<", 2))
			{
				expand_env->result[expand_env->j++] = *expand_env->token++;
				continue ;
			}
			else
			{
				process_variable(expand_env);
				continue ;
			}
		}
		expand_env->result[expand_env->j++] = *expand_env->token++;
	}
}

char	*expand_env_variables_in_token(const char *token, t_env *env,
		t_parse_cmd *parse_cmd)
{
	t_expand_env	expand_env;
	
	ft_bzero(&expand_env, sizeof(t_expand_env));
	expand_env.token = token;
	expand_env.env = env;
	expand_env.parse_cmd = parse_cmd;
	if (!token || !env)
		return (ft_strdup(""));
	parse_cmd->arr_has_dollar_count = 0;
	expand_env.result = ft_calloc(calculate_length(expand_env.token, expand_env.env, expand_env.parse_cmd) + 1,
			sizeof(char));
	expand_env.parse_cmd->arr_has_dollar_count = 0;
	if (parse_cmd->splitter_clean_input[parse_cmd->index_splitter]
		&& is_dolloar_quote(token) == is_dolloar_quote(parse_cmd->splitter_clean_input[parse_cmd->index_splitter])
		&& is_dolloar_quote(token) > 0)
		calculate_dollar_array(parse_cmd);
	if (!expand_env.result)
		return (NULL);
	expand_loop(&expand_env);	
	parse_cmd->arr_has_dollar_count = 0;
	if (parse_cmd->arr_has_dollar)
	{
		free(parse_cmd->arr_has_dollar);
		parse_cmd->arr_has_dollar = NULL;
	}
	expand_env.result[expand_env.j] = '\0';
	return (expand_env.result);
}
