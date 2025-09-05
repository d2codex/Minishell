#include "minishell.h"
// this test assums all tokens have already been passed through tokenizer
// and normalized
//
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
