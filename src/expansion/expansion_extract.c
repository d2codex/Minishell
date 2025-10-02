#include "minishell.h"

// this was a norminette hell, I'm sorry for all the helper functions !!

/**
 * @brief Validate the first character of a variable name.
 *
 * @param c Character to validate
 * @return true if valid start character (letter or underscore), false otherwise
 */
static bool	is_valid_var_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

/**
 * @brief Calculate the length of a variable name starting at given position.
 *
 * @param str Source string
 * @param start_pos Position after the '$' character
 * @return Length of the variable name (number of valid characters)
 */
static size_t	calculate_var_name_length(const char *str, size_t start_pos)
{
	size_t	i;

	i = start_pos;
	while (str[i])
	{
		if (!ft_isalpha(str[i]) && !ft_isdigit(str[i]) && str[i] != '_')
			break ;
		i++;
	}
	return (i - start_pos);
}

/**
 * @brief Extract variable name from a string starting at a given position.
 *
 * Extracts the variable name after a '$' character. A valid variable name
 * must start with a letter (a-z, A-Z) or underscore (_), and can continue
 * with letters, digits, or underscores.
 *
 * Special cases:
 * - $?  → returns "?" (exit status)
 * - $$  → returns "$" (PID, if we want to do bonuses)
 * We need to add this conditions if we want to handle $$ later :
 * 		if (str[start_pos + 1] == '$')
 * 			return (ft_strdup("$"));
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
 */
char	*extract_var_name(const char *str, size_t start_pos)
{
	size_t	var_len;
	char	*trimmed_name;

	if (!str || str[start_pos] != '$')
		return (NULL);
	if (str[start_pos + 1] == '?')
		return (ft_strdup("?"));
	if (ft_isdigit(str[start_pos + 1]))
		return (NULL);
	if (!str[start_pos + 1] || !is_valid_var_start(str[start_pos + 1]))
		return (NULL);
	var_len = calculate_var_name_length(str, start_pos + 1);
	trimmed_name = malloc(var_len + 1);
	if (!trimmed_name)
		return (NULL);
	ft_strlcpy(trimmed_name, str + start_pos + 1, var_len + 1);
	return (trimmed_name);
}

/**
 * @brief Retrieve the value of an environment variable by name.
 *
 * Searches for the specified variable in the shell's environment list and
 * returns a newly allocated copy of its value. Handles special variables
 * like the exit status ($?) and follows bash behavior for undefined variables
 * (returns empty string instead of error).
 *
 * Special cases:
 * - Variable name "?" returns the shell's last exit status as a string
 * - Undefined variables return an empty string (bash-compatible behavior)
 * - Variables with empty values return an empty string
 *
 * @param var_name Name of the variable to look up (without the '$' prefix)
 * @param data Shell data structure containing the environment list and status
 * @return Newly allocated string containing the variable's value, or empty
 *         string if variable not found. Never returns NULL.
 *
 * @note Caller is responsible for freeing the returned string
 * @note Always returns an allocated string, even for errors or missing variables
 * @note The returned string must be freed to avoid memory leaks
 *
 * Examples:
 *   get_var_value("USER", data)      // returns "john" if USER=john
 *   get_var_value("?", data)         // returns "0" if last command succeeded
 *   get_var_value("UNDEFINED", data) // returns "" (empty string)
 *   get_var_value("EMPTY", data)     // returns "" if EMPTY= (empty value)
 */
char	*get_var_value(const char *var_name, t_shell *data)
{
	t_env	*env_node;

	if (!var_name || !data || var_name[0] == '\0')
		return (ft_strdup(""));
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(data->status));
	env_node = get_env_node_by_key(data->env_list, var_name);
	if (env_node && env_node->value)
		return (ft_strdup(env_node->value));
	return (ft_strdup(""));
}
