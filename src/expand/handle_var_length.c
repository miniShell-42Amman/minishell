/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var_length.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:04:30 by oissa             #+#    #+#             */
/*   Updated: 2025/03/09 22:15:22 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	function_else_check(const char **token, t_expand_env *expand,
		size_t *var_len)
{
	if (expand->parse_cmd->arr_has_dollar
		&& expand->parse_cmd->arr_has_dollar[
			expand->parse_cmd->arr_has_dollar_count] != (size_t)-1)
	{
		*var_len = expand->parse_cmd->arr_has_dollar[
			expand->parse_cmd->arr_has_dollar_count];
		*token += *var_len;
		expand->parse_cmd->arr_has_dollar_count++;
	}
	else
	{
		while (**token && (ft_isalnum(**token) || **token == '_')
			&& **token != '\'')
		{
			(*token)++;
		}
	}
}

size_t	handle_special_cases(const char **token, t_expand_env *expand,
		char **value)
{
	char	var_name[2];

	var_name[0] = **token;
	var_name[1] = '\0';
	if (**token == '?')
	{
		*value = ft_itoa(*expand->parse_cmd->exit_status);
		(*token)++;
		return (1);
	}
	else if (**token == '0')
	{
		*value = ft_strdup(expand->parse_cmd->program_name);
		(*token)++;
		return (1);
	}
	else if (ft_isdigit(**token))
	{
		*value = get_var_value(expand->env, var_name, 1);
		(*token)++;
		return (1);
	}
	return (0);
}

size_t	fill_value(char *value, int should_free, size_t *len)
{
	if (value)
	{
		*len = ft_strlen(value);
		if (should_free)
			free(value);
		return (*len);
	}
	return ((size_t)0);
}

size_t	handle_var_length(const char **token, t_expand_env *expand)
{
	const char	*start;
	char		*value;
	size_t		var_len;
	int			should_free;
	size_t		len;

	value = NULL;
	should_free = 0;
	var_len = 0;
	start = *token;
	if (handle_special_cases(token, expand, &value))
	{
		should_free = 1;
	}
	else
	{
		function_else_check(token, expand, &var_len);
		var_len = *token - start;
		value = get_var_value(expand->env, start, var_len);
		should_free = 0;
	}
	len = fill_value(value, should_free, &len);
	return (len);
}
