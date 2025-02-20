/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:30:37 by oissa             #+#    #+#             */
/*   Updated: 2025/02/16 14:56:12 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid(int *array, int token_index)
{
	int	len;
	int	i;

	if (array == NULL)
		return (0);
	len = 0;
	while (array[len])
		len++;
	i = 0;
	while (i < len)
	{
		if (array[i] == token_index)
			return (1);
		i++;
	}
	return (0);
}

t_token_type	determine_token_type(char *token, int token_index,
		t_token *tokens_list, int *array)
{
	if (ft_strcmp(token, "|") == 0 && !is_valid(array, token_index))
		return (TOKEN_PIPE);
	else if (ft_strcmp(token, "<") == 0 && !is_valid(array, token_index))
		return (TOKEN_REDIRECTION_IN);
	else if (ft_strcmp(token, ">") == 0 && !is_valid(array, token_index))
		return (TOKEN_REDIRECTION_OUT);
	else if (ft_strcmp(token, ">>") == 0 && !is_valid(array, token_index))
		return (TOKEN_REDIRECTION_APPEND);
	else if (ft_strcmp(token, "<<") == 0 && !is_valid(array, token_index))
		return (TOKEN_REDIRECTION_HEREDOC);
	else if (token_index == 0 || (token_index > 0
			&& tokens_list[token_index - 1].type == TOKEN_PIPE))
		return (TOKEN_COMMAND);
	else
		return (TOKEN_ARGUMENT);
}

static int	store_token_value(t_token *new_token, char **tokens_list, int i)
{
	int	j;

	if (tokens_list[i] == NULL)
	{
		new_token[i].value = NULL;
		return (EXIT_SUCCESS);
	}
	new_token[i].value = ft_strdup(tokens_list[i]);
	if (!new_token[i].value)
	{
		j = 0;
		while (j < i)
		{
			if (new_token[j].value)
				free(new_token[j].value);
			j++;
		}
		free(new_token);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	free_tokens(t_token *tokens, int token_count)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (i < token_count)
	{
		if (tokens[i].value)
		{
			free(tokens[i].value);
			tokens[i].value = NULL;
		}
		i++;
	}
	free(tokens);
	tokens = NULL;
}

t_token	*store_token(char **tokens_list, int token_count, int *array)
{
	t_token	*new_token;
	int		i;

	new_token = ft_calloc(sizeof(t_token), token_count);
	if (!new_token)
		return (NULL);
	i = -1;
	while (++i < token_count)
	{
		if (tokens_list[i] == NULL)
		{
			new_token[i].value = NULL;
			new_token[i].type = TOKEN_ARGUMENT;
			// i++;
			continue ;
		}
		if (store_token_value(new_token, tokens_list, i))
		{
			free_tokens(new_token, i);
			return (NULL);
		}
		new_token[i].type = determine_token_type(tokens_list[i], i, new_token,
				array);
	}
	return (new_token);
}
