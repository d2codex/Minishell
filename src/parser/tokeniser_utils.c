#include "minishell.h"

t_quote	update_quote_state(t_quote current_quote_state, char c)
{
	if (c == '\'' && current_quote_state == STATE_NORMAL)
		current_quote_state = STATE_IN_SINGLE_QUOTE;
	else if (c == '"' && current_quote_state == STATE_NORMAL)
		current_quote_state = STATE_IN_DOUBLE_QUOTE;
	else if (c == '\'' && current_quote_state == STATE_IN_SINGLE_QUOTE)
		current_quote_state = STATE_NORMAL;
	else if (c == '"' && current_quote_state == STATE_IN_DOUBLE_QUOTE)
		current_quote_state = STATE_NORMAL;
	return (current_quote_state);
}

int	is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (true);
	return (false);
}

bool	is_a_shell_separator(t_quote current_quote_state, char c)
{
	if (current_quote_state != STATE_NORMAL)
		return (false);
	if (is_whitespace(c))
		return (true);
	if ((c == '|' || c == '<' || c == '>'))
		return (true);
	return (false);
}

bool	has_unclosed_quotes(char const *s)
{
	size_t	i;
	t_quote	current_quote_state;

	if (!s)
		return (false);
	i = 0;
	current_quote_state = STATE_NORMAL;
	while (s[i])
	{
		current_quote_state = update_quote_state(current_quote_state, s[i]);
		i++;
	}
	if (current_quote_state != STATE_NORMAL)
		return (true);
	else
		return (false);
}

int	count_shell_tokens(char const *s)
{
	size_t	i;
	size_t	count;
	size_t	in_word;
	t_quote	current_quote_state;

	i = 0;
	count = 0;
	in_word = 0;
	current_quote_state = STATE_NORMAL;
	while (s[i] != '\0')
	{
		current_quote_state = update_quote_state(current_quote_state, s[i]);
		if (!is_a_shell_separator(current_quote_state, s[i]) && !in_word)
		{
			in_word = 1;
			count += 1;
		}
		else if (is_a_shell_separator(current_quote_state, s[i]) && in_word)
			in_word = 0;
		i++;
	}
	return (count);
}
