#include "minishell.h"

static int is_valid_operator(char *token, int pos, char **tokens)
{
    if (ft_strchr(token, '\'') || ft_strchr(token, '"'))
        return (1); 
    if (ft_strcmp(token, "|") == 0) {
        if (pos == 0 || !tokens[pos+1] || ft_strcmp(tokens[pos-1], "|") == 0)
            return (0);
        return (1);
    }
    if (ft_strcmp(token, ">") == 0 || ft_strcmp(token, "<") == 0 ||
        ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0) {
        if (!tokens[pos+1] || ft_strchr("><|", tokens[pos+1][0]))
            return (0);
        return (1);
    }
    return (1);
}

static void validate_tokens(char **tokens, t_counter *ct)
{
	int i;

    i = -1;
    while (tokens[++i]) {
        if (ft_strchr("|<>", tokens[i][0]) && !is_valid_operator(tokens[i], i, tokens)) {
            if (!ct->array)
                ct->array = ft_calloc(ct->count_word + 1, sizeof(int));
            ct->array[ct->count++] = i;
        }
    }
    if (ct->array)
        ct->array[ct->count] = -1;
}

int *ft_count_token(char *input)
{
    t_counter ct;
    char **tokens;

    ft_bzero(&ct, sizeof(t_counter));
    tokens = smart_split(input);
    if (!tokens)
        return (NULL);
    while (tokens[ct.count_word])
        ct.count_word++;
    validate_tokens(tokens, &ct);
    ft_free_split(tokens);
    return ct.array;
}
