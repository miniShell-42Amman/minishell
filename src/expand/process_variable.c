/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_variable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 22:24:23 by oissa             #+#    #+#             */
/*   Updated: 2025/03/08 22:39:23 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	array_has_dolloar(t_expand_env *e, char **value, size_t *var_len,
		int *should_free)
{
	if (e->parse_cmd->arr_has_dollar
		&& e->parse_cmd->arr_has_dollar[
			e->parse_cmd->arr_has_dollar_count] != (size_t) - 1)
	{
		*var_len = e->parse_cmd->arr_has_dollar[
			e->parse_cmd->arr_has_dollar_count];
		e->token += *var_len;
		e->parse_cmd->arr_has_dollar_count++;
		*value = get_var_value(e->env, e->start, *var_len);
		if (*value && **value && ft_strchr(*value, ' '))
			e->parse_cmd->must_splitter[
				e->parse_cmd->index_splitter] = (size_t)1;
		else
			e->parse_cmd->must_splitter[
				e->parse_cmd->index_splitter] = (size_t)0;
		*should_free = 0;
	}
}

void	else_check_expand(t_expand_env *expand, char **value, size_t *var_len,
		int *should_free)
{
	while (*expand->token && (ft_isalnum(*expand->token)
			|| *expand->token == '_') && *expand->token != '\'')
		expand->token++;
	*var_len = (size_t)(expand->token - expand->start);
	*value = get_var_value(expand->env, expand->start, *var_len);
	if (**value && ft_strchr(*value, ' '))
		expand->parse_cmd->must_splitter[
			expand->parse_cmd->index_splitter] = (size_t)1;
	else
		expand->parse_cmd->must_splitter[
			expand->parse_cmd->index_splitter] = (size_t)0;
	*should_free = 0;
}

void	if_value(t_expand_env *expand, char **value, int *should_free)
{
	if (*value)
	{
		ft_strlcpy(expand->result + expand->j, *value, ft_strlen(*value) + 1);
		expand->j += ft_strlen(*value);
		if (*should_free)
			free(*value);
	}
}

void	process_variable(t_expand_env *expand)
{
	char	*value;
	size_t	var_len;
	int		should_free;
	char	var_name[2] = {*expand->token, '\0'};
	
	value = NULL;
	should_free = 0;
	var_len = 0;
	expand->token++;
	expand->start = expand->token;
	if (*expand->token == '?')
		value = get_data(expand, var_ft_itoa, var_name, &should_free);
	else if (*expand->token == '0')
		value = get_data(expand, var_ft_strdup, var_name, &should_free);
	else if (ft_isdigit(*expand->token))
		value = get_data(expand, var_get_var_value, var_name, &should_free);
	else if (expand->parse_cmd->arr_has_dollar
		&& expand->parse_cmd->arr_has_dollar[
			expand->parse_cmd->arr_has_dollar_count] != (size_t)-1)
		array_has_dolloar(expand, &value, &var_len, &should_free);
	else
		else_check_expand(expand, &value, &var_len, &should_free);
	if_value(expand, &value, &should_free);
}
