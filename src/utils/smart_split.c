#include "minishell.h"

size_t ft_array_size(char **array)
{
	size_t i = 0;

	while (array[i])
		i++;
	return (i);
}

int count_quoted_words(const char *str)
{
	t_split s;
	ft_bzero(&s, sizeof(s));
	while (str[s.i])
	{
		update_quote_state(str[s.i], &s.sq, &s.dq);
		if ((!s.sq && !s.dq) && (str[s.i] == ' ' || str[s.i] == '|' ||
								 str[s.i] == '<' || str[s.i] == '>'))
		{
			if (s.i > s.start)
				s.wc++;
			if (str[s.i] == '|' || str[s.i] == '<' || str[s.i] == '>')
				s.wc++;
			s.start = s.i + 1;
		}
		s.i++;
	}
	if (s.i > s.start)
		s.wc++;
	return (s.wc);
}

char **smart_split(const char *str)
{
	char **res;
	t_split s;
	const char *original;
	int word_count;

	original = str;
	ft_bzero(&s, sizeof(t_split));
	word_count = count_quoted_words(str);
	res = (char **)ft_calloc(word_count + 1, sizeof(char *));
	if (!res)
		return (NULL);
	while (str[s.i])
	{
		update_quote_state(str[s.i], &s.sq, &s.dq);
		if ((!s.sq && !s.dq) && (str[s.i] == ' ' || str[s.i] == '|' ||
								 str[s.i] == '<' || str[s.i] == '>'))
		{
			if (s.i > s.start)
				res[s.wc++] = ft_substr(original, s.start, s.i - s.start);
			if (str[s.i] == '|' || str[s.i] == '<' || str[s.i] == '>')
				res[s.wc++] = ft_substr(original, s.i, (str[s.i + 1] == str[s.i]) ? 2 : 1);
			s.start = s.i + ((str[s.i + 1] == str[s.i]) ? 2 : 1);
			if (str[s.i + 1] == str[s.i])
				s.i++;
		}
		s.i++;
	}
	if (s.i > s.start)
		res[s.wc++] = ft_substr(original, s.start, s.i - s.start);
	res[s.wc] = NULL;
	return (res);
}