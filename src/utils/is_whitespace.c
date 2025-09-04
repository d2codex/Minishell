#include "minishell.h"

/**
 * @brief Check if a character is a whitespace.
 *
 * Whitespace characters: space, tab, newline, vertical tab,
 * form feed, carriage return.
 *
 * @param c Character to check
 * @return 1 if whitespace, 0 otherwise
 */
bool	is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (true);
	return (false);
}
