#include "minishell.h"

/**
 * @brief Calculate the size needed for a single variable expansion.
 *
 * Helper function that extracts a variable name, looks up its value,
 * calculates the size needed for replacement, and advances the position
 * pointer past the entire variable token ($ + name).
 *
 * @param str The input string being analyzed
 * @param i Pointer to current position - will be advanced past the variable
 * @param data Shell data containing environment variables
 * @return Size in bytes needed for the variable's expanded value
 *
 * @note The position pointer i is modified to skip past the entire variable
 * @note Memory allocated internally is freed before return
 */
static size_t	get_variable_size(const char *str, size_t *i, t_shell *data)
{
	char	*var_name;
	char	*value;
	size_t	var_size;
	size_t	name_len;

	var_name = extract_var_name(str, *i);
	if (!var_name)
	{
		(*i)++; // Skip invalid '$' to avoid infinite loop
		return (1); // Count '$' as literal character (bash behavior)
	}
	value = get_var_value(var_name, data);
	var_size = ft_strlen(value);
	name_len = ft_strlen(var_name);
	*i += name_len + 1; // Skip '$' + variable name in main loop
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
		// Calculate size needed for this variable expansion
		if (str[i] == '$' && should_expand_at_position(str, i))
		{
			var_size = get_variable_size(str, &i, data);
			total_size += var_size;
		}
		else
		{
			// Regular character: count as 1 byte
			total_size++;
			i++;
		}
	}
	return (total_size);
}

/**
 * @brief Copy a variable's expanded value into the result buffer.
 *
 * Helper function that looks up a variable's value and copies it
 * character by character into the result buffer at the specified position.
 *
 * @param result Destination buffer to write into
 * @param dst_pos Starting position in the destination buffer
 * @param var_name Name of variable to expand (without '$' prefix)
 * @param data Shell data for variable lookup
 * @return Number of characters copied into the buffer
 *
 * @note Automatically frees the looked-up value after copying
 */
static size_t	copy_variable_value(char *result, size_t dst_pos,
	const char *var_name, t_shell *data)
{
	char	*var_value;
	size_t	j;

	var_value = get_var_value(var_name, data);
	j = 0;
	while (var_value[j])
	{
		result[dst_pos + j] = var_value[j];
		j++;
	}
	free(var_value);
	return (j);
}

/**
 * @brief Fill pre-allocated buffer with expanded version of input string.
 *
 * Second pass of the two-pass expansion algorithm. Assumes the result
 * buffer has been allocated with exactly the right size (calculated by
 * calculate_expanded_size). Scans the input and copies regular characters
 * as-is while replacing variables with their expanded values.
 *
 * @param result Pre-allocated buffer to fill (must be correctly sized)
 * @param str Source string containing variables to expand
 * @param data Shell data structure containing environment variables
 *
 * @note Result buffer MUST be pre-allocated with sufficient size
 * @note The result string is automatically null-terminated
 * @note Variables in single quotes are copied literally, not expanded
 */
void	fill_expanded_string(char *result, const char *str, t_shell *data)
{
	size_t	src_pos;
	size_t	dst_pos;
	char	*var_name;

	src_pos = 0;
	dst_pos = 0;
	while (str[src_pos])
	{
		if (str[src_pos] == '$' && should_expand_at_position(str, src_pos))
		{
			// Extract and expand the variable
			var_name = extract_var_name(str, src_pos);
			if (!var_name)
				return ; // Should not happen after calculate_expanded_size
			// Copy variable's value into result buffer
			dst_pos += copy_variable_value(result, dst_pos, var_name, data);
			// Skip past the entire variable token ($VAR)
			src_pos += ft_strlen(var_name) + 1;
			free(var_name);
		}
		else
		{
			// Copy regular character as is
			result[dst_pos] = str[src_pos];
			dst_pos++;
			src_pos++;
		}
	}
	result[dst_pos] = '\0';
}

/**
 * @brief Expand all environment variables in a string.
 *
 * Main entry point for variable expansion. Uses a two-pass algorithm:
 * 1. Calculate the exact size needed for the expanded result
 * 2. Allocate precisely that amount and fill with expanded content
 *
 * This approach is more efficient than incremental string building as it
 * requires only one memory allocation and avoids repeated reallocations.
 *
 * @param str Input string containing variables to expand (e.g., "$USER")
 * @param data Shell data structure containing environment variables
 * @return Newly allocated string with variables expanded, or NULL on error
 *
 * @note Caller is responsible for freeing the returned string
 * @note Returns empty string for zero-length expansion results
 * @note Variables in single quotes are preserved literally
 *
 * Examples:
 *   expand_variables_in_string("hello $USER", data)     // "hello john"
 *   expand_variables_in_string("'$USER'", data)         // "'$USER'"
 *   expand_variables_in_string("status: $?", data)      // "status: 0"
 */
char	*expand_variables_in_string(const char *str, t_shell *data)
{
	char	*result;
	size_t	final_size;

	if (!str || !data)
		return (NULL);
	final_size = calculate_expanded_size(str, data);
	if (final_size == 0)
		return (ft_strdup(""));
	result = malloc(sizeof(char) * (final_size + 1));
	if (!result)
		return (NULL);
	fill_expanded_string(result, str, data);
	return (result);
}
