#include "minishell.h"

/**
 * @brief Check if a variable at a given position should be expanded.
 *
 * Variables are expanded everywhere except inside single quotes.
 * This function tracks the quote state from the beginning of the string
 * up to the target position to determine the context.
 *
 * @param str The string to analyze
 * @param pos Position to check (should be a '$' character)
 * @return true if expansion should occur at this position, false otherwise
 *
 * Examples:
 *   should_expand_at_position("$USER", 0)     → true
 *   should_expand_at_position("'$USER'", 1)   → false (single quotes)
 *   should_expand_at_position("\"$USER\"", 1) → true (double quotes)
 */
bool	should_expand_at_position(const char *str, size_t pos)
{
	size_t		i;
	t_quote	current_quote_state;

	if (!str || str[pos] != '$')
		return (false);
	i = 0;
	current_quote_state = STATE_NOT_IN_QUOTE;
	while (i <= pos)
	{
		// update the current quote state based on the character we're looking at
		current_quote_state = update_quote_state(current_quote_state, str[i]);
		i++;
	}
	if (current_quote_state == STATE_NOT_IN_QUOTE
		|| current_quote_state == STATE_IN_DOUBLE_QUOTE)
		return (true);
	else
		return (false);
}
