#include "minishell.h"

/**
 * @brief "State machine" for the quotes.
 * Update the current quote state based on a character.
 *
 * This function updates the shell's quote state when encountering
 * single or double quote characters.
 *
 * Used by: has_unclosed_quotes(), extract_operator(), extract_word(),
 *
 * @param current_quote_state The current state (NORMAL, SINGLE, DOUBLE)
 * @param c The character to process
 * @return The updated quote state
 */
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

/**
 * @brief Check if a character is a shell separator.
 *
 * A separator is a whitespace or a special shell operator (|, <, >)
 * when not inside quotes. Inside quotes, metacharacters are preserved
 * and not treated as separators.
 *
 * Helper for: extract_word(), count_word_token()
 *
 * @param current_quote_state The current quote state
 * @param c The character to check
 * @return true if the character is a separator, false otherwise
 */
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

/**
 * @brief Detect if the string has unclosed quotes.
 *
 * Scans the string and checks if there is any quote that is
 * not closed at the end of the string.
 *
 * Used in: ft_split_tokens() to reject invalid input early
 *
 * @param s The string to check
 * @return true if there are unclosed quotes, false otherwise
 */
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

/**
 * @brief Skip consecutive whitespace characters in a string.
 *
 * Advances the index pointer until a non-whitespace character is found.
 * This ensures that no empty tokens are counted when parsing shell input.
 *
 * @param s The input string to parse.
 * @param i Pointer to the current index in the string. Will be updated.
 */
void	skip_whitespace(char const *s, size_t *i)
{
	while (s[*i] && is_whitespace(s[*i]))
		(*i)++;
}
