/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 22:46:25 by oissa             #+#    #+#             */
/*   Updated: 2025/02/21 00:22:53 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int env_lstsize(t_env *lst);
void env_lstadd_back(t_env **lst, t_env *new);
t_env *env_lstnew(char *key, char *value);
char *remove_quotes(char *str);
void free_str_array(char **arr);
void ft_sort_string_arr(char **arr, int size);
char *ft_strjoin3(const char *s1, const char *s2, const char *s3);
int export_cmd(char **args, int arg_count, t_env **env);
void print_env_list(t_env *env);

int env_lstsize(t_env *lst)
{
    int count = 0;
    while (lst)
    {
        count++;
        lst = lst->next;
    }
    return count;
}

char *remove_quotes(char *str)
{
    int len = ft_strlen(str);
    if (len < 2)
        return ft_strdup(str);
    if ((str[0] == '"' && str[len - 1] == '"') ||
        (str[0] == '\'' && str[len - 1] == '\''))
    {
        char *clean = ft_substr(str, 1, len - 2);
        return clean;
    }
    return ft_strdup(str);
}

static void print_export_error(char *arg)
{
    ft_putstr_fd("minishell: export: `", STDERR_FILENO);
    ft_putstr_fd(arg, STDERR_FILENO);
    ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

void env_lstadd_back(t_env **lst, t_env *new) {
    if (!*lst) {
        *lst = new;
        printf("Added first node: %s=%s\n", new->key, new->value);
        return;
    }
    t_env *last = *lst;
    while (last->next)
        last = last->next;
    last->next = new;
    printf("Added new node: %s=%s\n", new->key, new->value);
}

t_env *env_lstnew(char *key, char *value)
{
    t_env *new = malloc(sizeof(t_env));
    if (!new)
        return NULL;
    
    new->key = ft_strdup(key);
    new->value = value ? ft_strdup(value) : NULL;
    new->has_value = (value != NULL);
    
    if (!new->key || (value && !new->value))
    {
        free(new->key);
        free(new->value);
        free(new);
        return NULL;
    }
    new->next = NULL;
    return new;
}

char *ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
    char *tmp = ft_strjoin(s1, s2);
    if (!tmp)
        return NULL;
    char *result = ft_strjoin(tmp, s3);
    free(tmp);
    return result;
}

void ft_sort_string_arr(char **arr, int size)
{
    int i, j;
    char *tmp;
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - i - 1; j++) {
            if (ft_strcmp(arr[j], arr[j + 1]) > 0) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void free_str_array(char **arr)
{
    int i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

static int is_valid_identifier(char *str)
{
    int i = 0;
    if (str[i] == '"' || str[i] == '\'')
        i++;
    if (!ft_isalpha(str[i]) && str[i] != '_')
        return 0;
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_' && str[i] != '"' && str[i] != '\'')
            return 0;
        i++;
    }
    return 1;
}

static char **env_to_sorted_array(t_env *env)
{
    int count = env_lstsize(env);
    char **arr = malloc(sizeof(char *) * (count + 1));
    if (!arr)
        return NULL;
    int i = 0;
    t_env *tmp = env;
    while (tmp)
    {
        if (tmp->value)
            arr[i] = ft_strjoin3(tmp->key, "=", tmp->value);
        else
            arr[i] = ft_strdup(tmp->key);
        tmp = tmp->next;
        i++;
    }
    arr[i] = NULL;
    ft_sort_string_arr(arr, count);
    return arr;
}

static int print_sorted_env(t_env *env)
{
    char **env_arr = env_to_sorted_array(env);
    if (!env_arr)
        return EXIT_FAILURE;
    int i = 0;
    while (env_arr[i])
    {
        ft_putstr_fd("declare -x ", STDOUT_FILENO);
        ft_putendl_fd(env_arr[i], STDOUT_FILENO);
        free(env_arr[i]);
        i++;
    }
    free(env_arr);
    return EXIT_SUCCESS;
}

static void update_env_var(t_env **env, char *key, char *value)
{
    t_env *current = *env;
    char *new_key = ft_strdup(key);
    char *new_value = value ? ft_strdup(value) : NULL;
    if (!new_key || (value && !new_value))
    {
        free(new_key);
        free(new_value);
        return;
    }
    while (current)
    {
        if (ft_strcmp(current->key, new_key) == 0)
        {
            free(current->value);
            current->value = new_value;
            free(new_key);
            return;
        }
        current = current->next;
    }
    if (!new_value)
        printf("New value is NULL\n");
    else
        printf("New value: %s\n", new_value);
    t_env *new_node = env_lstnew(new_key, new_value);
    printf("Adding key: %s, value: %s\n", new_key, new_value ? new_value : "(null)");
    free(new_key);
    free(new_value);
    env_lstadd_back(env, new_node);
}

void print_env_list(t_env *env) {
    t_env *tmp = env;
    while (tmp) {
        printf("Env node: key = %s, value = %s\n", 
               tmp->key, tmp->value ? tmp->value : "(null)");
        tmp = tmp->next;
    }
}


int export(char **args, int arg_count, t_env **env)
{
    if (arg_count == 1)
        return print_sorted_env(*env);
    int status = EXIT_SUCCESS;
    for (int i = 1; i < arg_count; i++)
    {
        char *clean_arg = remove_quotes(args[i]);
        if (!is_valid_identifier(clean_arg))
        {
            print_export_error(args[i]);
            status = EXIT_FAILURE;
            free(clean_arg);
            continue;
        }
        char *eq_pos = strchr(clean_arg, '=');
        char *key = eq_pos ? ft_substr(clean_arg, 0, eq_pos - clean_arg) : ft_strdup(clean_arg);
        char *value = eq_pos ? ft_strdup(eq_pos + 1) : NULL;
        if (!key || (eq_pos && !value))
        {
            free(clean_arg);
            free(key);
            free(value);
            status = EXIT_FAILURE;
            continue;
        }
        update_env_var(env, key, value);
        free(key);
        free(value);
        free(clean_arg);
    }
    print_env_list(*env);
    return status;
}

