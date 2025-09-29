#include "minishell.h"

/**
 * @brief   Identify positions in the input string where variable expansion should occur.
 *
 * This function scans through the string while tracking the current quote state.
 * It detects `$` characters and determines whether they should trigger expansion
 * based on whether they appear outside of quotes, inside double quotes,
 * or inside single quotes (where expansion is disabled).
 *
 * @param str Input string to analyze (can be NULL).
 *
 * @note Currently only prints the decision for each `$` found.
 *       Intended as a helper for debugging / future expansion logic.
 */
void	identify_expansions(const char *str)
{
	int		i;
	t_quote	current_quote_state;

	if (!str)
		return ;
	i = 0;
	current_quote_state = STATE_NOT_IN_QUOTE;
	while (str[i])
	{
		// update the current quote state based on the character we're looking at
		current_quote_state = update_quote_state(current_quote_state, str[i]);

		// whenever we encounter a '$', we need to decide if it should expand
		if (str[i] == '$')
		{
			// outside quotes or inside double quotes -> expand
			if (current_quote_state == STATE_NOT_IN_QUOTE
				|| current_quote_state == STATE_IN_DOUBLE_QUOTE)
				printf("found '$' at position %d, should expand: YES\n", i);

			// inside single quotes -> do not expand
			else if (current_quote_state == STATE_IN_SINGLE_QUOTE)
				printf("found '$' at position %d, should expand: NO\n", i);
		}
		i++;
	}
}

int main(void)
{
	printf("Test 1: No quotes\n");
	identify_expansions("hello $USER");

	printf("\nTest 2: Double quotes\n");
	identify_expansions("\"$USER\"");

	printf("\nTest 3: Single quotes\n");
	identify_expansions("'$USER'");

	printf("\nTest 4: Mixed\n");
	identify_expansions("\"$USER\" and '$HOME'");

	return (0);
}
