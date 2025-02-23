/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 01:24:11 by lalhindi          #+#    #+#             */
/*   Updated: 2025/02/23 01:34:22 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_key(char *s)
{
	int	i;

	i = 0;
	if (!ft_isalpha(s[i]) && s[i] != '_')
		return (0);
	i++;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	print_export(t_env *env)
{
	char	**arr;
	int		i;

	arr = convert_env_to_list(env);
	if (!arr)
		return (1);
	ft_sort_arr(arr, ft_list_size(env));
	i = -1;
	while (arr[++i])
	{
		ft_printf("declare -x %s\n", arr[i]);
		free(arr[i]);
	}
	free(arr);
	return (0);
}

static int	handle_invalid_key(char *arg)
{
	ft_printf("export: `%s': not a valid identifier\n", arg);
	return (1);
}

static int	split_key_value(char *arg, char **key, char **value)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		*key = ft_substr(arg, 0, eq - arg);
		if (!*key)
			return (0);
		*value = ft_strdup(eq + 1);
		if (!*value)
		{
			free(*key);
			return (0);
		}
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
		if (!*key)
			return (0);
	}
	return (1);
}

int	export(char **args, int args_count, t_env **env)
{
	char	*arg;
	char	*k;
	char	*v;
	int		status;
	int		i;

	status = 0;
	if (args_count == 1)
		return (print_export(*env));
	i = 0;
	while (++i < args_count)
	{
		arg = remove_quotes(args[i]);
		if (!arg || !is_valid_key(arg))
			status += handle_invalid_key(args[i]);
		else if (!split_key_value(arg, &k, &v))
			status += 1;
		else
			update_env(env, k, v);
		free(arg);
	}
	return (status > 0);
}
