/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 21:10:02 by oissa             #+#    #+#             */
/*   Updated: 2025/02/02 00:20:39 by oissa            ###   ########.fr       */
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
#include <readline/readline.h>
#include <readline/history.h>
#include "../Libft/libft.h"

// typedef struct s_data
// {
//     char **env;
//     char **path;
//     char *home;
//     char *oldpwd;
//     char *pwd;
//     int status;
//     char *line;
//     char **cmd;
// }               t_data;

typedef struct s_cmd
{
    char *cmd;
    char **args;
    int arg_count;
}               t_cmd;

/*
    ! Token structures
*/
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
    char quote_char;
}       t_parse_cmd;

/*
    ! Token functions
*/
t_cmd parse_cmd(char *input);
char *trim_spaces(char *str);
int count_args(char *input);

/*
    ! Utils functions
*/
void free_command(t_cmd *cmd);

#endif