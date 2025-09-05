#include "minishell.h"

/**
 * @brief Checks if a given token is a valid shell environment variable key.
 *
 * A valid key must:
 *  - Not be NULL or empty
 *  - Start with a letter (a-z, A-Z) or underscore (_)
 *  - Contain only alphanumeric characters (a-z, A-Z, 0-9) or underscores (_) 
 *    up to an optional '=' character
 *
 * @param token The input string representing the token to validate.
 * @return true if the token is a valid key according to the rules, false
 * otherwise.
 */
bool	is_valid_key(const char *token)
{
	int	i;

	// if token is null or empty
	if (!token || !*token)
		return (false);
	// first character can only be letters or '_'
	// therefore the '=' check is included here if its the first char
	if (!ft_isalpha(token[0]) && token[0] != '_')
		return (false);
	// remaining can be alphanumeric or '_'
	i = 1;
	while (token[i] && token[i] != '=')
	{
		if (!ft_isalnum(token[i]) && token[i] != '_')
			return (false);
		i++;
	}
	return (true);
}
