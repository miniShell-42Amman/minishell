/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 21:45:29 by oissa             #+#    #+#             */
/*   Updated: 2025/02/21 16:52:51 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_env_var(t_env *env, const char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return (env->value);
        env = env->next;
    }
    return (NULL);
}

void set_env_var(t_env **env, const char *key, const char *value)
{
    t_env *tmp = *env;

    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
        {
            free(tmp->value);
            tmp->value = ft_strdup(value);
            return;
        }
        tmp = tmp->next;
    }
    add_new_node(env, new_node_env(ft_strdup(key), ft_strdup(value)));
}

void print_cd_error(const char *cmd, const char *arg)
{
    ft_dprintf(2, "minishell: %s: %s\n", cmd, arg);
}

static int update_pwd(t_env **env, const char *oldpwd)
{
    char cwd[PATH_MAX];

    if (!getcwd(cwd, sizeof(cwd)))
    {
        print_cd_error("cd", "failed to get current directory");
        return (1);
    }
    set_env_var(env, "OLDPWD", oldpwd);
    set_env_var(env, "PWD", cwd);
    return (0);
}

static char *get_target_dir(char **args, t_env *env)
{
    if (!args[1])
        return get_env_var(env, "HOME");
    if (ft_strcmp(args[1], "-") == 0)
        return get_env_var(env, "OLDPWD");
    return args[1];
}

void cd(char **args, int arg_count, t_env **env)
{
    char oldpwd[PATH_MAX];
    char *target;

    if (arg_count > 2)
    {
        print_cd_error("cd", "too many arguments");
        return;
    }
    if (!getcwd(oldpwd, sizeof(oldpwd)))
    {
        print_cd_error("cd", "failed to get current directory");
        return;
    }
    target = get_target_dir(args, *env);
    if (!target)
    {
        print_cd_error("cd", args[1] ? "OLDPWD not set" : "HOME not set");
        return;
    }
    if (chdir(target) != 0)
    {
        print_cd_error("cd", "failed to change directory");
        return;
    }
    if (update_pwd(env, oldpwd) == 0 && args[1] && ft_strcmp(args[1], "-") == 0)
        ft_dprintf(2,get_env_var(*env, "PWD"));
}
