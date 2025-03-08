#include "minishell.h"

// size_t	handle_var_length(t_expand_env *expand)
// {
// 	const char	*start;
// 	char		*value;
// 	size_t		var_len;
// 	int			should_free;
// 	char var_name[2] = {*expand->token, '\0'};
// 	size_t		len;

// 	value = NULL;
// 	should_free = 0;
// 	var_len = 0;
// 	start = expand->token;
// 	if (*expand->token == '?')
// 	{
// 		value = ft_itoa(*expand->parse_cmd->exit_status);
// 		should_free = 1;
// 		expand->token++;
// 	}
// 	else if (*expand->token == '0')
// 	{
// 		value = ft_strdup(expand->parse_cmd->program_name);
// 		should_free = 1;
// 		expand->token++;
// 	}
// 	else if (ft_isdigit(*expand->token))
// 	{
// 		value = get_var_value(expand->env, var_name, 1);
// 		should_free = 1;
// 		expand->token++;
// 	}
// 	else
// 	{
// 		if (expand->parse_cmd->arr_has_dollar
// 			&& expand->parse_cmd->arr_has_dollar[expand->parse_cmd->arr_has_dollar_count] != (size_t)-1)
// 		{
// 			var_len = expand->parse_cmd->arr_has_dollar[expand->parse_cmd->arr_has_dollar_count];
// 			expand->token += var_len;
// 			expand->parse_cmd->arr_has_dollar_count++;
// 		}
// 		else
// 		{
// 			while (*expand->token && (ft_isalnum(*expand->token) || *expand->token == '_')
// 				&& *expand->token != '\'')
// 				expand->token++;
// 		}
// 		var_len = expand->token - start;
// 		value = get_var_value(expand->env, start, var_len);
// 		should_free = 0;
// 	}
// 	if (value)
// 	{
// 		len = ft_strlen(value);
// 		if (should_free)
// 			free(value);
// 		return (len);
// 	}
// 	return (0);
// }