#include "minishell.h"

/**
 * @brief Count a shell operator token at the current index.
 *
 * Recognizes the following operators as single tokens:
 *  - Pipe: |
 *  - Redirections: <, >, <<, >>
 *
 * Advances the index past the operator.
 *
 * Helper for: count_shell_tokens()
 *
 * @param s The input string to parse.
 * @param i Pointer to the current index in the string. Will be updated.
 * @return int 1 if an operator token was counted, 0 otherwise.
 */
static int	count_operator_at_index(const char *s, size_t *i)
{
	if (s[*i] == '|' || s[*i] == '<' || s[*i] == '>')
	{
		if ((s[*i] == '<' || s[*i] == '>') && s[*i + 1] && s[*i + 1] == s[*i])
			*i += 2;
		else
			(*i)++;
		return (1);
	}
	return (0);
}

/**
 * @brief Count a word or quoted token starting at the current index.
 *
 * A word token is a sequence of characters that is not a shell separator.
 * Handles quoted strings, updating the quote state as it parses.
 * Advances the index until a shell separator or the end of the string.
 *
 * Helper for: count_shell_tokens()
 *
 * @param s The input string to parse.
 * @param i Pointer to the current index in the string.
 * @return int Always returns 1 token counted.
 */
static int	count_word_at_index(const char *s, size_t *i)
{
	t_quote	quote_state;

	quote_state = STATE_NORMAL;
	while (s[*i] && !is_a_shell_separator(quote_state, s[*i]))
	{
		quote_state = update_quote_state(quote_state, s[*i]);
		(*i)++;
	}
	return (1);
}

/**
 * @brief Count the total number of shell tokens in a command string.
 *
 * Parses the input string character by character:
 * 1. Skips leading whitespace.
 * 2. Checks for operators and counts them as tokens.
 * 3. Counts word tokens (including quoted strings) as tokens.
 * 4. Repeats until the end of the string.
 *
 * This function ensures that the token count is in sync with
 * extract_tokens_to_tab() to avoid desynchronization.
 *
 * @param s The input shell command string.
 * @return int Total number of tokens found.
 */
int	count_shell_tokens(const char *s)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		skip_whitespace(s, &i);
		if (!s[i])
			break ;
		if (count_operator_at_index(s, &i))
			count++;
		else
			count += count_word_at_index(s, &i);
	}
	return (count);
}
