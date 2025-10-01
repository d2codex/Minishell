#include "minishell.h"
/**
 * @brief Extract variable name from a string starting at a given position.
 *
 * Extracts the variable name after a '$' character. A valid variable name
 * must start with a letter (a-z, A-Z) or underscore (_), and can continue
 * with letters, digits, or underscores.
 *
 * Special cases:
 * - $?  → returns "?" (exit status)
 * - $$  → returns "$" (PID, bonus)
 * - $123 → returns NULL (starts with digit, invalid)
 * - $   → returns NULL (dollar alone, no variable)
 *
 * @param str The string containing the variable
 * @param start_pos Position of the '$' character
 * @return Allocated string with variable name (without $), or NULL if invalid
 *
 * Examples:
 *   extract_var_name("$USER", 0)        → "USER"
 *   extract_var_name("$USER world", 0)  → "USER"
 *   extract_var_name("$_VAR", 0)        → "_VAR"
 *   extract_var_name("$?", 0)           → "?"
 *   extract_var_name("$123", 0)         → NULL
 * TODO : split into helpers for the norm
 */
char	*extract_var_name(const char *str, size_t start_pos)
{
	int		var_len;
	char	*trimmed_name;
	int		i;
	int		j;

	// Basic validation: ensure string exists and starts with '$'
	if (!str || str[start_pos] != '$')
		return (NULL);

	// Special case: $? expands to exit status
	if (str[start_pos + 1] == '?')
		return (ft_strdup("?"));

	// Special case: $$ expands to process ID (bonus feature)
	/* if (str[start_pos + 1] == '$')
		return (ft_strdup("$")); */

	// Invalid: variable name cannot start with a digit
	// Example: $123 is not a valid variable
	if (ft_isdigit(str[start_pos + 1]))
		return (NULL);

	// Invalid cases:
	// - '$' alone (no character after)
	// - Character after '$' is neither letter nor underscore
	if ((!str[start_pos + 1]) || ((!ft_isalpha(str[start_pos + 1]))
		&& (str[start_pos + 1] != '_')))
		return (NULL);

	// Calculate variable name length
	// Start from position after '$' and continue while characters are valid
	// Valid characters: letters (a-z, A-Z), digits (0-9), underscore (_)
	i = start_pos + 1;
	while (str[i])
	{
		// Stop at first character that's not alphanumeric or underscore
		if (!ft_isalpha(str[i]) && !ft_isdigit(str[i]) && (str[i] != '_'))
			break ;
		i++;
	}

	// Calculate length: distance from start to end, minus the '$' itself
	// Example: "$USER" → i=5, start_pos=0 → length = 5 - 0 - 1 = 4
	var_len = i - start_pos - 1;

	// Allocate memory for the variable name (without the '$')
	trimmed_name = malloc((sizeof(char) * var_len) + 1);
	if (!trimmed_name)
		return (NULL);

	// Copy the variable name character by character
	// Start from position after '$' (start_pos + 1)
	// Copy exactly var_len characters (the variable name length)
	j = 0;
	i = start_pos + 1;
	while (j < var_len)
	{
		trimmed_name[j] = str[i];
		j++;
		i++;
	}

	// Null-terminate the string
	trimmed_name[j] = '\0';

	return (trimmed_name);
}

void test_extract(const char *input, size_t pos)
{
	char *result = extract_var_name(input, pos);
	printf("Input: \"%s\", pos: %zu → Result: [%s]\n",
			input, pos, result ? result : "NULL");
	if (result) free(result);
}

int main(void)
{
	test_extract("$USER", 0);
	test_extract("$USER world", 0);
	test_extract("$_VAR", 0);
	test_extract("$USER123", 0);
	test_extract("$?", 0);
	test_extract("$", 0);
	test_extract("$123", 0);
	test_extract("hello $USER!", 6);
	test_extract("'$HOME'", 1);
	test_extract("'$VAR@'", 1);
	return (0);
}
