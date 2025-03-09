#include "minishell.h"

static void	count_dollars(t_parse_cmd *p, int *j)
{
	int	i;

	i = 0;
	*j = 0;
	while (p->splitter_clean_input[p->index_splitter][i])
	{
		if (p->splitter_clean_input[p->index_splitter][i] == '$')
			(*j)++;
		i++;
	}
}

static void	process_dollars(t_parse_cmd *p, int *j, size_t *help)
{
	int	i;

	i = 0;
	*j = 0;
	*help = 0;
	while (p->splitter_clean_input[p->index_splitter][i])
	{
		if (p->splitter_clean_input[p->index_splitter][i] == '$')
		{
			if (p->splitter_clean_input[p->index_splitter][i + 1] == '?')
			{
				p->arr_has_dollar[(*j)++] = 1;
				i += 2;
				continue ;
			}
			*help = 0;
			i++;
			while (p->splitter_clean_input[p->index_splitter][i + *help]
				&& (ft_isalnum(p->splitter_clean_input[p->index_splitter][i + *help])
					|| p->splitter_clean_input[p->index_splitter][i + *help] == '_'))
				(*help)++;
			p->arr_has_dollar[(*j)++] = *help;
		}
		else
			i++;
	}
}

void		calculate_dollar_array(t_parse_cmd *p)
{
	int		j;
	size_t	help;

	count_dollars(p, &j);
	p->arr_has_dollar = malloc(sizeof(size_t) * (j + 1));
	if (!p->arr_has_dollar)
		return ;
	process_dollars(p, &j, &help);
	p->arr_has_dollar[j] = (size_t)-1;
}

int			is_string_inside_single(const char *token)
{
	int		i;
	int		qc;
	char	parent;

	i = 0;
	qc = 0;
	parent = 0;
	if (!token)
		return (0);
	while (token[i])
	{
		if (token[i] == '\'' || token[i] == '"')
		{
			if (!parent)
				parent = token[i];
			else if (parent == token[i])
				parent = 0;
			qc++;
		}
		i++;
	}
	if (qc % 2 != 0)
		return (0);
	return (parent == '\'');
}

static void	handle_heredoc_case(t_expand_env *ex)
{
	if (ex->parse_cmd->index_splitter >= 1 && ft_strnstr(
			ex->parse_cmd->splitter_clean_input[
			ex->parse_cmd->index_splitter - 1], "<<", 2))
	{
		ex->result[ex->j++] = *ex->token++;
		return ;
	}
	process_variable(ex);
}

static void	process_expansion(t_expand_env *ex)
{
	const char	*input;

	input = ex->parse_cmd->splitter_clean_input[
		ex->parse_cmd->index_splitter];
	update_quote_state(*ex->token, &ex->squote, &ex->dquote);
	if (*ex->token == '$' && !is_string_inside_single(input)
		&& *(ex->token + 1) != ' ' && *(ex->token + 1) != '\0')
		handle_heredoc_case(ex);
	else
		ex->result[ex->j++] = *ex->token++;
}

void		expand_loop(t_expand_env *ex)
{
	while (*ex->token)
		process_expansion(ex);
}

static void	cleanup_dollar_array(t_parse_cmd *p)
{
	if (p->arr_has_dollar)
	{
		free(p->arr_has_dollar);
		p->arr_has_dollar = NULL;
	}
}

char		*expand_env_variables_in_token(const char *token, t_env *env,
				t_parse_cmd *p)
{
	t_expand_env	ex;
	const char		*input;

	ft_bzero(&ex, sizeof(ex));
	ex.token = token;
	ex.env = env;
	ex.parse_cmd = p;
	if (!token || !env)
		return (ft_strdup(""));
	p->arr_has_dollar_count = 0;
	input = p->splitter_clean_input[p->index_splitter];
	ex.result = ft_calloc(calculate_length(&ex) + 1, sizeof(char));
	if (input && is_dolloar_quote(token) == is_dolloar_quote(input)
		&& is_dolloar_quote(token) > 0)
		calculate_dollar_array(p);
	if (!ex.result)
		return (NULL);
	expand_loop(&ex);
	p->arr_has_dollar_count = 0;
	cleanup_dollar_array(p);
	ex.result[ex.j] = '\0';
	return (ex.result);
}
