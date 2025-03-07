/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 21:45:29 by oissa             #+#    #+#             */
/*   Updated: 2025/03/07 23:16:40 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_env_var(t_env **env, const char *key, const char *value)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
	add_new_node(env, new_node_env(ft_strdup(key), ft_strdup(value)));
}

int	print_cd_error(const char *cmd, const char *arg)
{
	ft_dprintf(2, "⚠️  Error404 ⚠️ : %s: %s\n", cmd, arg);
	return (1);
}

static int	update_pwd(t_env **env, const char *oldpwd)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		print_cd_error("cd", "failed to get current directory");
		return (1);
	}
	set_env_var(env, "OLDPWD", oldpwd);
	set_env_var(env, "PWD", cwd);
	return (0);
}

static char	*get_target_dir(char **args, t_env *env)
{
	if (!args[1])
		return (get_env_var(env, "HOME"));
	if (ft_strcmp(args[1], "-") == 0)
		return (get_env_var(env, "OLDPWD"));
	return (args[1]);
}

int cd_if(char **args, char **target, t_env **env)
{
	*target = NULL;
	
	*target = get_target_dir(args, *env);
	if (!*target)
	{
		if (args[1])
			print_cd_error("cd", "OLDPWD not set");
		else
			print_cd_error("cd", "HOME not set");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	cd(char **args, int arg_count, t_env **env)
{
	char	oldpwd[PATH_MAX];
	char	*target;

	if (arg_count > 2 && print_cd_error("cd", "too many arguments"))
		return (1);
	if (!getcwd(oldpwd, sizeof(oldpwd)) && print_cd_error("cd",
			"failed to get current directory"))
		return (1);
	if (cd_if(args, &target, env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (ft_strcmp(oldpwd, target) == 0)
		return (0);
	if (chdir(target) != 0 && print_cd_error("cd",
			"failed to change directory"))
		return (1);
	if (update_pwd(env, oldpwd) == 0)
	{
		if (args[1] && ft_strcmp(args[1], "-") == 0)
			ft_dprintf(2, "%s\n", get_env_var(*env, "PWD"));
		return (0);
	}
	return (1);
}
