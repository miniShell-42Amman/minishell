#include "minishell.h"

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
	return (quote_count % 2 != 0 && parent == '\'');
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
	expand_env.result = ft_calloc(calculate_length(&expand_env) + 1,
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
