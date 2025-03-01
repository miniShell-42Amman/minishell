#include "minishell.h"
#include <stdbool.h>

void update_quote_state(char c, bool *squote, bool *dquote)
{
    if (c == '\'' && !*dquote)
        *squote = !*squote;
    else if (c == '"' && !*squote)
        *dquote = !*dquote;
}

size_t handle_var_length(const char **token, t_env *env, t_parse_cmd *p)
{
    const char *start;
    char *value;
    size_t var_len;

    var_len = 0;
    start = *token;
    if (**token == '?')
    {
        value = ft_itoa(*p->exit_status);
        (*token)++;
    }
    else
    {

        if (p->arr_has_dollar && p->arr_has_dollar[p->arr_has_dollar_count] != (size_t)-1)
        {
            var_len = p->arr_has_dollar[p->arr_has_dollar_count];
            *token += var_len;
            p->arr_has_dollar_count++;
        }
        else
        {

            while (**token && (ft_isalnum(**token) || **token == '_') && **token != '\'')
                (*token)++;
        }

        var_len = *token - start;
        value = get_var_value(env, start, var_len);
    }
    if (value)
        return (ft_strlen(value));
    return (0);
}

void process_variable(const char **t, t_env *e, char **res, size_t *j, t_parse_cmd *p)
{
    const char *start;
    char *value;
    size_t var_len;

    var_len = 0;
    (*t)++;
    start = *t;
    if (**t == '?')
    {
        value = ft_itoa(*p->exit_status);
        (*t)++;
    }

    else
    {
        if(ft_isdigit(**t))
        {
            value = ft_itoa(**t - '0');
            (*t)++;
        }
        else if(**t == '_')
        {
            value = get_var_value(e, "_", 1);
            (*t)++;
        }
        else if (p->arr_has_dollar && p->arr_has_dollar[p->arr_has_dollar_count] != (size_t)-1)
        {
            var_len = p->arr_has_dollar[p->arr_has_dollar_count];
            *t += var_len;
            p->arr_has_dollar_count++;
        }
        else
        {
            while (**t && (ft_isalnum(**t) || **t == '_') && **t != '\'')
                (*t)++;
        }
        var_len = (size_t)(*t - start);
        value = get_var_value(e, start, var_len);
    }
    if (value)
    {
        ft_strlcpy(*res + *j, value, ft_strlen(value) + 1);
        *j += ft_strlen(value);
    }
}

int is_dolloar_quote(const char *token)
{
    int i = 0;
    int count = 0;
    if (!token || !*token)
        return 0;
    while (token[i] && token[i] != ' ')
    {
        if (token[i] == '$')
            count++;
        i++;
    }
    return (count);
}

void calculate_dollar_array(t_parse_cmd *p)
{
    int i = 0;
    int j = 0;
    while (p->splitter_clean_input[p->index_splitter][i])
    {
        if (p->splitter_clean_input[p->index_splitter][i] == '$')
        {
            j++;
        }
        i++;
    }
    p->arr_has_dollar = malloc(sizeof(size_t) * (j + 1));
    if (!p->arr_has_dollar)
        return;
    i = 0;
    j = 0;
    size_t help = 0;
    while (p->splitter_clean_input[p->index_splitter][i])
    {
        if (p->splitter_clean_input[p->index_splitter][i] == '$')
        {
            if (p->splitter_clean_input[p->index_splitter][i + 1] == '?')
            {
                p->arr_has_dollar[j++] = 1;
                i += 2;
                continue;
            }
            help = 0;
            i++;
            while (p->splitter_clean_input[p->index_splitter][i + help] && (ft_isalnum(p->splitter_clean_input[p->index_splitter][i + help]) || p->splitter_clean_input[p->index_splitter][i + help] == '_'))
            {
                help++;
            }
            p->arr_has_dollar[j++] = help;
        }
        else
        {
            i++;
        }
    }
    p->arr_has_dollar[j] = (size_t)-1;
}
int is_string_inside_single(const char *token)
{
    int i = 0;
    int quote_count = 0;
    char parent = 0;

    if (!token)
        return 0;

    while (token[i])
    {
        if (token[i] == '\'' || token[i] == '"')
        {
            if (parent == 0)
                parent = token[i];
            else if (parent == token[i])
                parent = 0;
            quote_count++;
        }
        i++;
    }
    if (quote_count % 2 != 0)
        return 0;
    return (parent == '\'') ? 1 : 0;
}

char *expand_env_variables_in_token(const char *token, t_env *env, t_parse_cmd *parse_cmd)
{
    char *result;
    size_t j;
    bool squote;
    bool dquote;
    j = 0;
    if (!token || !env)
        return (ft_strdup(""));
    parse_cmd->arr_has_dollar_count = 0;
    result = ft_calloc(calculate_length(token, env, parse_cmd) + 1, sizeof(char));
    parse_cmd->arr_has_dollar_count = 0;
    if (parse_cmd->splitter_clean_input[parse_cmd->index_splitter] && is_dolloar_quote(token) == is_dolloar_quote(parse_cmd->splitter_clean_input[parse_cmd->index_splitter]) && is_dolloar_quote(token) > 0)
        calculate_dollar_array(parse_cmd);
    if (!result)
        return (NULL);
    j = 0;
    squote = false;
    dquote = false;
    while (*token)
    {
        update_quote_state(*token, &squote, &dquote);
        if (*token == '$' && !is_string_inside_single(parse_cmd->splitter_clean_input[parse_cmd->index_splitter]) && (*(token + 1) != ' ' && *(token + 1) != '\0'))
        {
            process_variable(&token, env, &result, &j, parse_cmd);
            continue;
        }
        result[j++] = *token++;
    }
    parse_cmd->arr_has_dollar_count = 0;
    if (parse_cmd->arr_has_dollar)
    {
        free(parse_cmd->arr_has_dollar);
        parse_cmd->arr_has_dollar = NULL;
    }
    result[j] = '\0';
    return (result);
}
