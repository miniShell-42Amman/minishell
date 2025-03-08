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

t_env	*new_node_env(char *key, char *value)
{
	t_env	*new_node;

	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	return (new_node);
}

void	add_new_node(t_env **head, t_env *new_node)
{
	t_env	*tmp;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

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
	var_name = ft_calloc(len + 1, sizeof(char));
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
	free(var_name);
	return (NULL);
}

size_t	calculate_length(const char *token, t_env *env, t_parse_cmd *p)
{
	size_t	len;
	bool	squote;
	bool	dquote;

	len = 0;
	squote = false;
	dquote = false;
	if (*p->splitter_clean_input && p->splitter_clean_input[p->index_splitter]
		&& is_dolloar_quote(token) == is_dolloar_quote(p->splitter_clean_input[p->index_splitter])
		&& is_dolloar_quote(token) > 0)
		calculate_dollar_array(p);
	while (*token)
	{
		update_quote_state(*token, &squote, &dquote);
		if (*token == '$'
			&& !is_string_inside_single(p->splitter_clean_input[p->index_splitter])
			&& (*(token + 1) != ' ' && *(token + 1) != '\0'))
		{
			token++;
			len += handle_var_length(&token, env, p);
			continue ;
		}
		len++;
		token++;
	}
	if (p->arr_has_dollar)
	{
		free(p->arr_has_dollar);
		p->arr_has_dollar = NULL;
	}
	return (len);
}
