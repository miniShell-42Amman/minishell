#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ft_is_operator(const char *str)
{
    if (*str == '|')
        return 1;
    if (*str == '>' || *str == '<')
    {
        if (*(str + 1) == *str)
            return 2;
        return 1;
    }
    return 0;
}

int count_args(char *input)
{
    int count = 0;
    int i = 0;
    int len = strlen(input);

    while (i < len)
    {
        while (i < len && (input[i] == ' ' || input[i] == '\t'))
            i++;
        if (i >= len)
            break;
        count++;
        int in_single = 0;
        int in_double = 0;
        while (i < len)
        {
            if (input[i] == '\'' && !in_double)
                in_single = !in_single;
            else if (input[i] == '"' && !in_single)
                in_double = !in_double;

            if (!in_single && !in_double)
            {
                if (input[i] == ' ' || input[i] == '\t' || input[i] == '|' || ft_is_operator(&input[i]))
                    break;
            }
            i++;
        }
        i++;
    }
    return count;
}

static int get_operator_length(char *input, int i)
{
    if (input[i] == '|')
        return 1;
    if (input[i] == '<' || input[i] == '>')
    {
        if (input[i + 1] == input[i])
            return 2;
        return 1;
    }
    return 0;
}

static int is_inside_quotes(int in_single, int in_double)
{
    return (in_single || in_double);
}

static int is_adjacent_to_quotes(char *input, int i, int op_len)
{
    if (i > 0 && (input[i - 1] == '\'' || input[i - 1] == '"'))
        return 1;
    if (input[i + op_len] == '\'' || input[i + op_len] == '"')
        return 1;
    return 0;
}

int *ft_count_token(char *input)
{
    if (!input || input[0] == '\0' || input[0] == '|')
        return NULL;

    int *array = NULL;
    int count = 0, i = 0, count_word = 0;
    int in_single_quotes = 0, in_double_quotes = 0;

    while (input[i])
    {
        while (input[i] == ' ' || input[i] == '\t')
            i++;
        if (!input[i])
            break;
        count_word++;

        while (input[i] && ((input[i] != ' ' && input[i] != '\t') || in_single_quotes || in_double_quotes))
        {
            if (input[i] == '\'' && !in_double_quotes)
                in_single_quotes = !in_single_quotes;
            else if (input[i] == '"' && !in_single_quotes)
                in_double_quotes = !in_double_quotes;

            int op_len = get_operator_length(input, i);
            if (op_len > 0)
            {
                if (is_inside_quotes(in_single_quotes, in_double_quotes) || 
                    is_adjacent_to_quotes(input, i, op_len))
                {
                    if (!array)
                        array = malloc(sizeof(int) * (count_args(input) + 1));
                    array[count++] = count_word - 1;
                }
                i += op_len;
                continue;
            }
            i++;
        }
    }

    if (array)
        array[count] = -1;
    return array;
}

int main()
{
    char *input = "ls -l \'|\' cat -e \"|\" wc -l | grep -i";
    int *array = ft_count_token(input);
    if (array)
    {
        int i = 0;
        while (array[i] != -1)
        {
            printf("%d\n", array[i]);
            i++;
        }
        free(array);
    }
    return 0;
}