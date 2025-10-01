#include "minishell.h"

/**
 * @brief Remove outer quotes from a string
 *
 * Only removes quotes if:
 * - String starts with ' or "
 * - String ends with the SAME quote character
 * - String length > 1
 *
 * Examples:
 *   "hello"  → hello
 *   'world'  → world
 *   hello	→ hello (no quotes, return copy)
 *   "hello'  → "hello' (mismatched, return copy)
 *
 * @param str Input string
 * @return Newly allocated string without outer quotes, or NULL on error
 */
char *trim_quotes(const char *str)
{
	int		str_len;
	int		trimmed_len;
	char	*trimmed_str;

	if (!str || str[0] == '\0')
		return (ft_strdup(""));
	str_len = ft_strlen(str);
	if ((str[0] == '"' && str[str_len - 1] == '"')
		|| (str[0] == '\'' && str[str_len - 1] == '\''))
	{
		// case where you have "" or ''
		if (str_len == 2)
			return (ft_strdup(""));
		trimmed_len = str_len - 2;
		// malloc done internally into substring
		trimmed_str = ft_substr(str, 1, trimmed_len);
	}
	else
		trimmed_str = ft_strdup(str);
	return (trimmed_str);
}

int main(void)
{
	char *result;

	// Test 1: Double quotes
	result = trim_quotes("\"hello\"");
	printf("Test 1: %s (expected: hello)\n", result);
	free(result);

	// Test 2: Single quotes
	result = trim_quotes("'world'");
	printf("Test 2: %s (expected: world)\n", result);
	free(result);

	// Test 3: No quotes
	result = trim_quotes("hello");
	printf("Test 3: %s (expected: hello)\n", result);
	free(result);

	// Test 4: Mismatched
	result = trim_quotes("\"hello'");
	printf("Test 4: %s (expected: \"hello')\n", result);
	free(result);

	// Test 5: Empty quotes
	result = trim_quotes("\"\"");
	printf("Test 5: '%s' (expected: empty string)\n", result);
	free(result);

	// Test 6: Empty quotes (edge case)
	result = trim_quotes("\"\"");
	printf("Test 6: '%s' (expected: empty string)\n", result);
	free(result);

	result = trim_quotes("''");
	printf("Test 7: '%s' (expected: empty string)\n", result);
	free(result);

	return 0;
}
