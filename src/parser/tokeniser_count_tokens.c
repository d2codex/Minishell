#include "minishell.h"

/**
 * @brief Skip whitespace characters in the string.
 *
 * Advances the index until a non whitespace character is found.
 *
 * @param s The string to parse
 * @param i Pointer to the current index in the string
 */
void	skip_whitespace(char const *s, size_t *i)
{
	while (s[*i] && is_whitespace(s[*i]))
		(*i)++;
}

/**
 * @brief Count a token if it is a shell operator.
 *
 * Handles |, <, >, <<, >> as single tokens and advances the index.
 * Only counts as operator if we're in STATE_NORMAL.
 *
 * @param s The string to parse
 * @param i Pointer to the current index
 * @return 1 if an operator token was found, 0 otherwise
 */
static int	count_operator_token(char const *s, size_t *i,
	t_quote current_quote_state)
{
	int	count;

	count = 0;
	if (current_quote_state == STATE_NORMAL &&
		(s[*i] == '|' || s[*i] == '<' || s[*i] == '>'))
	{
		count = 1;
		if ((s[*i] == '<' || s[*i] == '>') && s[*i + 1] == s[*i])
			*i += 2;
		else
			(*i)++;
	}
	return (count);
}

/**
 * @brief Count a word token or quoted sequence.
 *
 * Advances the index until a shell separator is found.
 *
 * @param s The string to parse
 * @param i Pointer to the current index
 * @param current_quote_state Current quote state
 * @return 1 token counted
 */
static int	count_word_token(char const *s, size_t *i,
	t_quote current_quote_state)
{
	while (s[*i] && !is_a_shell_separator(current_quote_state, s[*i]))
	{
		current_quote_state = update_quote_state(current_quote_state, s[*i]);
		(*i)++;
	}
	return (1);
}

/**
 * @brief Count the total number of shell tokens in a command line.
 *
 * Tokens include words, quoted sequences, and operators: |, <, >, <<, >>.
 * Spaces are ignored, quotes are respected.
 *
 * @param s The command line string
 * @return The total number of tokens
 */
int	count_shell_tokens(char const *s)
{
	size_t	i;
	size_t	count;
	t_quote	current_quote_state;
	int		tmp;

	if (!s)
		return (0);
	i = 0;
	count = 0;
	current_quote_state = STATE_NORMAL;
	while (s[i])
	{
		skip_whitespace(s, &i);
		if (!s[i])
			break ;
		current_quote_state = update_quote_state(current_quote_state, s[i]);
		tmp = count_operator_token(s, &i, current_quote_state);
		if (tmp == 0)
			tmp = count_word_token(s, &i, current_quote_state);
		count += tmp;
	}
	return (count);
}
