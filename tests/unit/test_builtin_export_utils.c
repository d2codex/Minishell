/**
 * @file test_is_valid_key.c
 * @brief Test program for is_valid_key() function.
 *
 * This test verifies that the is_valid_key() function correctly identifies
 * valid and invalid environment variable identifiers. Tokens are assumed
 * to have been passed through a tokenizer and normalized.
 *
 * The test covers:
 * - Valid keys with uppercase/lowercase letters, digits, and underscores
 * - Invalid keys starting with digits, containing spaces, '=' at start,
 *   or illegal characters (e.g., '-')
 * - Edge cases such as empty strings or "=" only
 *
 * Compile:
 *      make build TEST=unit/test_is_valid_key.c
 *
 * Usage:
 *      ./bin/test_is_valid_key
 *
 * Expected output:
 * - For each token, prints:
 *      - Token itself
 *      - Result of is_valid_key() (TRUE/FALSE)
 *      - Expected result
 * - Colored output using RED/GRN/BLU macros
 *
 * Notes:
 * - Boolean results are printed as TRUE/FALSE strings.
 * - Sentinel NULL at the end of the test array terminates the loop.
 */

#include "minishell.h"

typedef struct s_test
{
	const char	*token;
	bool		expected;
}	t_test;

int	main(void)
{
	int		i;
	bool	result;
	char	*result_str;
	char	*expected_str;
	char	*result_color;

	t_test	tests[] = {
	{"FOO=bar", true},
	{"foo=BAR", true},
	{"FOOZ=BAZ", true},
	{"Foo=Bar", true},
	{"foo=bar", true},
	{"_UNDERSCORE=ok", true},
	{"VAR123=456", true},
	{"HELLO_WORLD=hi", true},
	{"=invalid_key", false},
	{"1FOO=bar", false},
	{"FOO-BAR=bad", false},
	{"FOO BAR=nospaces", false},
	{"", false},
	{"=", false},
	// add more tests here
	{NULL, false} // sentinel
	};
	
	printf("=========================IS VALID KEY TEST=======================\n");
	i = 0;
	while (tests[i].token)
	{
		result = is_valid_key(tests[i].token);
		if (result)
		{
			result_str = "TRUE";
			result_color = GRN;
		}
		else
		{
			result_str = "FALSE";
			result_color = RED;
		}
		if (tests[i].expected)
		{
			expected_str = "TRUE";
			result_color = GRN;
		}
		else
		{
			expected_str = "FALSE";
			result_color = RED;
		}
		printf(BLU "[%d] " YEL "%s" RESET "\n", i, tests[i].token);
		printf("is_valid_key: " "%s%s" RESET "\n", result_color, result_str);
		printf("expected    : " "%s%s" RESET "\n\n", result_color, expected_str);
		i++;
	}
	printf("=============================END TEST============================\n");
}
