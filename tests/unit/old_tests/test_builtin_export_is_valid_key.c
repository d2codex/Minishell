/**
 * @file test_builtin_export_is_valid_key.c
 * @brief Test program for is_valid_key() function.
 *
 * This test program validates that environment variable tokens conform to
 * the rules for valid identifiers in the shell:
 * - Must start with a letter or underscore
 * - May contain letters, digits, and underscores
 * - Cannot start with a digit
 * - Cannot contain spaces or symbols like '-'
 * - Empty strings or strings starting with '=' are invalid
 *
 * Compile:
 *      make build TEST=unit/test_builtin_export_is_valid_key.c
 *
 * Usage examples:
 * 1. Run all tests:
 *      ./bin/test_builtin_export_is_valid_key
 *    Expected output:
 *      - Each test prints:
 *          - Input token
 *          - is_valid_key result (TRUE/FALSE)
 *          - Expected result (TRUE/FALSE)
 *          - Colored output: green for TRUE, red for FALSE
 *
 * Notes:
 * - This test assumes tokens have already been normalized and preprocessed.
 * - The test array includes valid identifiers, invalid identifiers, and
 *   edge cases like empty strings or tokens starting with '='.
 * - The test loop stops at the sentinel {NULL, false}.
 * - Colored output uses RED/GRN/BLU/YEL/RESET macros.
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
