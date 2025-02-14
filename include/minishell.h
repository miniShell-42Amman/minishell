/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 21:10:02 by oissa             #+#    #+#             */
/*   Updated: 2025/02/07 21:55:24 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define RESET "\033[0m"
# define PURPLE "\033[0;35m"
# define WHITE "\033[0;37m"
# define BLUE "\033[0;34m"
# define YELLOW "\033[0;33m"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <sys/types.h>

/*
    ? env structures
    *   linked list to store the environment variables
*/
typedef struct s_env
{
    char *key;
    char *value;
    bool has_value;
    struct s_env *next;
}               t_env;


typedef struct s_cmd
{
    char *cmd;
    char **args;
    int arg_count;
    
}               t_cmd;

/*
    ? Token structures
*/
typedef enum e_token_type {
    TOKEN_COMMAND,
    TOKEN_ARGUMENT,             
    TOKEN_REDIRECTION_IN,       //*     "<"
    TOKEN_REDIRECTION_OUT,      //*     ">"
    TOKEN_REDIRECTION_APPEND,   //*     ">>"
    TOKEN_REDIRECTION_HEREDOC,  //*     "<<"
    TOKEN_PIPE
} t_token_type;


typedef struct s_token
{
    char *value;
    t_token_type type;
}       t_token;

typedef struct s_parse_cmd
{
    t_cmd cmd;
    char *clean_input;
    char *buffer;
    int i;
    int j;
    int k;
    bool in_quotes;
    bool token_started;
    bool token_was_single_quoted;
    bool token_was_dollar_quote;
    char *trimmed_input;
    char quote_char;
    char c;
    char    token_quote_type;
    int operator;
}       t_parse_cmd;

typedef struct s_main
{
    char *input;
    t_cmd *cmd;
    t_token *tokens_list;
    t_env *env_list;
}      t_main;


typedef struct s_counter {
    int     i;
    int     count_word;
    int     len;
    int     in_single;
    int     in_double;
    int     *array;
    int     count;
} t_counter;
/*
    * Functions
*/
int create_node(t_env **node);
int init_values(t_env *new_node, char **object);
t_env *clone_env(char **env);
int splitter_object(char **object, char *str, bool *has_value);
void  free_env_list(t_env *env_list);
int	add_node_to_env(t_env **head, t_env *new_node);
void	free_object(char **object, t_env *head);
int *ft_count_token(char *input);
int count_args(char *input);
t_cmd *parse_cmd(char *input, t_env *env_list);
char *find_env_value(t_env *env_list, const char *var_name);
char *expand_env_variables_in_token(const char *token, t_env *env_list);
void free_command(t_cmd *cmd);
t_token_type determine_token_type(char *token, int token_index, t_token *tokens_list, int *array);
t_token *store_token(char **tokens_list, int token_count, int *array);
void	free_tokens(t_token *tokens, int token_count);
// void ft_free_parse(t_parse_cmd *parse_cmd, t_cmd *cmd_result);
int	ft_is_operator(const char *str);
void	skip_whitespace(const char *input, int *i, int len);
// /*
//     ! Token functions
// */
// t_cmd parse_cmd(char *input, t_env *env_list);
// int count_args(char *input);
// char *expand_env_variables_in_token(char *token, t_env *env_list);
// int *ft_count_token(char *input);
// int is_valid(int *array, int token_index);
// t_token_type determine_token_type(char *token, int token_index, t_token *tokens_list, int *array);
// t_token *store_token(char **tokens_list, int token_count, int *array);



// /*
//     ! Utils functions
// */
// void free_command(t_cmd *cmd);
// void  free_env(t_env *env_list);
// void free_env_list(t_env *env_list);


// /*
//     ! Env functions
// */
// t_env *save_env(char **env);
// void add_node(t_env **head, t_env *new_node);
// char *find_env_value(t_env *env_list, const char *var_name);

#endif