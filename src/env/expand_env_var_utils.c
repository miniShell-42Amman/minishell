/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:16:48 by oissa             #+#    #+#             */
/*   Updated: 2025/02/15 17:16:48 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_env_value(t_env *env_list, const char *var_name)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return ("");
}

char	*get_var_value(t_env *env, const char *var, size_t len)
{
	char	*var_name;
	size_t	i;

	i = 0;
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	while (i < len)
	{
		var_name[i] = var[i];
		i++;
	}
	var_name[i] = '\0';
	while (env)
	{
		if (ft_strcmp(env->key, var_name) == 0)
		{
			free(var_name);
			return (env->value);
		}
		env = env->next;
	}
	
	return (NULL);
}

size_t	calculate_length(const char *token, t_env *env, size_t **len_var, t_parse_cmd *parse_cmd)
{
	size_t	len;
	bool	squote;
	bool	dquote;
	(void)parse_cmd;
	len = 0;
	squote = false;
	dquote = false;
	if (token[0] == '\'' && token[ft_strlen(token) - 1] == '\'' && is_dolloar_quote(token) > 1 && token[1] != '"')
	{
		char *tmp = ft_substr(token, 1, ft_strlen(token) - 2);
		ft_printf("Token after removing quotes: %s\n", tmp);
		// char *non_const_token = ft_strdup(token);
		// free(non_const_token);
		// token = ft_strdup(tmp);
		// free(tmp);
		token = tmp;
	}
	while (*token)
	{
		update_quote_state(*token, &squote, &dquote);
		if (*token == '$' && !squote)
		{
			token++;
			len += handle_var_length(&token, env, len_var);
			continue ;
		}
		len++;
		token++;
	}
	return (len);
}
