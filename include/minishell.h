/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 21:10:02 by oissa             #+#    #+#             */
/*   Updated: 2025/02/16 02:33:07 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"
#define PURPLE "\033[0;35m"
#define WHITE "\033[0;37m"
#define BLUE "\033[0;34m"
#define YELLOW "\033[0;33m"

#include "libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct s_env
{
	char *key;
	char *value;
	bool has_value;
	struct s_env *next;
} t_env;

typedef struct s_cmd
{
	char *cmd;
	char **args;
	int arg_count;

} t_cmd;

typedef enum e_token_type
{
	TOKEN_COMMAND,
	TOKEN_ARGUMENT,
	TOKEN_REDIRECTION_IN,
	TOKEN_REDIRECTION_OUT,
	TOKEN_REDIRECTION_APPEND,
	TOKEN_REDIRECTION_HEREDOC,
	TOKEN_PIPE
} t_token_type;

typedef struct s_token
{
	char *value;
	t_token_type type;
} t_token;

typedef struct s_parse_cmd
{
	t_cmd cmd;
	char *clean_input;
	char *buffer;
	int i;
	int j;
	int k;
	size_t *arr_has_dollar;
	char **splitter_clean_input;
	int index_splitter;
	size_t arr_has_dollar_count;
	size_t has_dollar;
	bool in_quotes;
	bool end_of_dollar;
	bool token_started;
	bool token_was_single_quoted;
	bool token_was_dollar_quote;
	char *trimmed_input;
	char quote_char;
	char c;
	char token_quote_type;
	int operator;
} t_parse_cmd;

typedef struct s_main
{
	char *input;
	t_cmd *cmd;
	t_token *tokens_list;
	t_env *env_list;
} t_main;

typedef struct s_split
{
	bool sq;
	bool dq;
	int start;
	int i;
	int wc;
} t_split;

typedef struct s_counter
{
	int i;
	int count_word;
	int len;
	int in_single;
	int in_double;
	int *array;
	int count;
} t_counter;

typedef struct s_execute
{
	char **envp;
	size_t num_cmds;
	char ***commands;
	size_t *cmd_args;
	int *pipe_fds;
	pid_t *pids;
	pid_t pid;
	int num_pipes;
	size_t cmd_index;
	size_t i;
	int j;
	size_t arg_index;
	char *cmd_path;
	t_env *env_list;
} t_execute;

int create_node(t_env **node);
int init_values(t_env *new_node, char **object);
t_env *clone_env(char **env);
int splitter_object(char **object, char *str, bool *has_value);
void free_env_list(t_env *env_list);
int add_node_to_env(t_env **head, t_env *new_node);
void free_object(char **object, t_env *head);
int *ft_count_token(char *input);
int count_args(char *input);
t_cmd *parse_cmd(char *input, t_env *env_list);
char *find_env_value(t_env *env_list, const char *var_name);
char *expand_env_variables_in_token(const char *token,
									t_env *env_list, t_parse_cmd *parse_cmd);
void free_command(t_cmd *cmd);
t_token_type determine_token_type(char *token, int token_index,
								  t_token *tokens_list, int *array);
t_token *store_token(char **tokens_list, int token_count,
					 int *array);
void free_tokens(t_token *tokens, int token_count);
int ft_is_operator(const char *str);
void skip_whitespace(const char *input, int *i, int len);
int free_cmd_parse(t_parse_cmd *parse_cmd, t_cmd *cmd_result);
t_cmd ft_free_parse_cmd(t_parse_cmd *parse_cmd);
int clean_parse_cmd(t_parse_cmd *parse_cmd);
int ft_have_operator(t_parse_cmd *parse_cmd);
int check_condition_too(t_parse_cmd *parse_cmd,
						t_env *env_list);
int check_condition(t_parse_cmd *parse_cmd);
int ft_check_parse_cmd(t_parse_cmd *parse_cmd);
int parse_cmd_loop(t_parse_cmd *parse_cmd, t_env *env_list);
int if_token_started_three(t_parse_cmd *parse_cmd,
						   t_env *env_list);
int if_token_started(t_parse_cmd *parse_cmd, t_env *env_list);
int init_parse_cmd(t_parse_cmd *parse_cmd, char *input);
char *get_var_value(t_env *env, const char *var, size_t len);
size_t calculate_length(const char *token, t_env *env, t_parse_cmd *p);
void update_quote_state(char c, bool *squote, bool *dquote);
size_t handle_var_length(const char **t, t_env *e, t_parse_cmd *p);
void process_variable(const char **t, t_env *e, char **res, size_t *j, t_parse_cmd *p);
void free_resources(t_main *main, int flag);
int is_duplicate_operator_series(t_token *token, int token_count);
int is_dolloar_quote(const char *token);
void calculate_dollar_array(t_parse_cmd *p);
int is_string_inside_single(const char *token);
char **convert_env_to_list(t_env *env_list);
void start_execution(t_token *tokens, size_t token_count, t_env *env_list);
char *ft_strtok(char *str, const char *delim);
int ft_strtok_utils(const char *delim, char **start, char **last);
int is_delim(char c, const char *delim);
int ft_list_size(t_env *env_list);
char **convert_env_to_list(t_env *env_list);
void free_execute(t_execute *execute);
void free_execute_too(t_execute *execute);
int fill_env_list(char ***envp, t_env *env_list, int envp_count);
void echo(char **args);
void cd(char **args, int arg_count, t_env **env);
void pwd(void);
void env(char **env);
int export(char **args, int arg_count, t_env **env);
void unset(char **args, t_env *env_list);
t_env *new_node_env(char *key, char *value);
void add_new_node(t_env **head, t_env *new_node);
void handle_redirections(t_execute *execute);


#endif
