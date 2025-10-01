#include "minishell.h"

/* this is a pure test, I won't use it in the final code */

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
		// update the current quote state based on the char we're looking at
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
