#include "minishell.h"

/**
 * @brief Check if a variable at a given position should be expanded.
 *
 * Variables are expanded everywhere except inside single quotes.
 * This function tracks the quote state from the beginning of the string
 * up to the target position to determine the context. It reuses the quote
 * state tracking logic from the tokenizer module for consistency.
 *
 * @param str The string to analyze
 * @param pos Position to check (should be a '$' character)
 * @return true if expansion should occur at this position, false otherwise
 *
 * Examples:
 *   should_expand_at_position("$USER", 0)     // returns true
 *   should_expand_at_position("'$USER'", 1)   // returns false (single quotes)
 *   should_expand_at_position("\"$USER\"", 1) // returns true (double quotes)
 */
bool	should_expand_at_position(const char *str, size_t pos)
{
	size_t		i;
	t_quote		current_quote_state;

	if (!str || str[pos] != '$')
		return (false);
	i = 0;
	current_quote_state = STATE_NOT_IN_QUOTE;
	while (i <= pos)
	{
		current_quote_state = update_quote_state(current_quote_state, str[i]);
		i++;
	}
	if (current_quote_state == STATE_NOT_IN_QUOTE
		|| current_quote_state == STATE_IN_DOUBLE_QUOTE)
		return (true);
	else
		return (false);
}

/**
 * @brief Calculate the size needed for a single variable expansion.
 *
 * Helper function that extracts a variable name, looks up its value,
 * calculates the size needed for replacement, and advances the position
 * pointer past the entire variable token ($ + name). If variable lookup
 * fails due to malloc error, returns 0 (empty variable).
 *
 * @param str The input string being analyzed
 * @param i Pointer to current position - will be advanced past the variable
 * @param data Shell data containing environment variables
 * @return Size in bytes needed for the variable's expanded value, or 0 if
 *         variable lookup fails or variable is empty
 *
 * @note The position pointer i is modified to skip past the entire variable
 * @note Memory allocated internally is freed before return
 * @note Returns 0 for undefined variables or malloc failures (bash behavior)
 */
size_t	get_variable_size(const char *str, size_t *i, t_shell *data)
{
	char	*var_name;
	char	*value;
	size_t	var_size;
	size_t	name_len;

	var_name = extract_var_name(str, *i);
	if (!var_name)
	{
		(*i)++;
		return (1);
	}
	value = get_var_value(var_name, data);
	if (!value)
	{
		free(var_name);
		name_len = ft_strlen(var_name);
		*i += name_len + 1;
		return (0);
	}
	var_size = ft_strlen(value);
	name_len = ft_strlen(var_name);
	*i += name_len + 1;
	free(var_name);
	free(value);
	return (var_size);
}

/**
 * @brief Calculate total size needed for string after variable expansion.
 *
 * First pass of a two-pass expansion algorithm. Scans the input string
 * to determine the exact number of bytes needed for the final expanded
 * string, allowing for optimal single memory allocation.
 *
 * @param str Input string containing variables to expand
 * @param data Shell data structure containing environment variables
 * @return Total bytes needed for expanded string
 *
 * @note This enables single malloc instead of multiple reallocations
 * @note Variables in single quotes are not counted for expansion
 */
size_t	calculate_expanded_size(const char *str, t_shell *data)
{
	size_t	total_size;
	size_t	i;
	size_t	var_size;

	total_size = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && should_expand_at_position(str, i))
		{
			var_size = get_variable_size(str, &i, data);
			total_size += var_size;
		}
		else
		{
			total_size++;
			i++;
		}
	}
	return (total_size);
}
