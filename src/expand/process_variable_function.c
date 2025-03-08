/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_variable_function.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 22:20:34 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/08 22:24:24 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*var_ft_itoa(t_expand_env *expand, const char *var_name)
{
	(void)var_name;
	return (ft_itoa(*expand->parse_cmd->exit_status));
}

char	*var_ft_strdup(t_expand_env *expand, const char *var_name)
{
	(void)var_name;
	return (ft_strdup(expand->parse_cmd->program_name));
}

char	*var_get_var_value(t_expand_env *expand, const char *var_name)
{
	return (get_var_value(expand->env, var_name, 1));
}

char	*get_data(t_expand_env *expand, t_var_func func, const char *var_name,
		int *should_free)
{
	char	*value;

	value = func(expand, var_name);
	*should_free = 1;
	expand->token++;
	return (value);
}
