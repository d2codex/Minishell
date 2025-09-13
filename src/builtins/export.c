#include "minishell.h"

/**
 * @brief Print an error for an invalid export key.
 *
 * Writes an error message to stderr if `errno == EINVAL`.
 *
 * @param token The invalid token.
 *
 * @return 0 if error was printed, -1 otherwise.
 */
static int	handle_invalid_key(const char *token)
{
	if (errno == EINVAL)
	{
		write(2, "$Hell: export: `", 17);
		write(2, token, ft_strlen(token));
		write(2, "': not a valid identifier\n", 26);
		return (0);
	}
	return (-1);
}

/**
 * @brief Handle environment variable updates for an existing key.
 *
 * Applies the correct action depending on the detected export operation:
 *  - EXPORT_NONE: mark the variable as declared but not exported
 *                 (env_node->in_env = false).
 *  - EXPORT_ASSIGN: replace the current value with the new one.
 *  - EXPORT_APPEND: concatenate the new value onto the existing one.
 *
 * @param env_node The environment node to update.
 * @param token    The input string containing the assignment or declaration.
 * @param op       The detected operation type (none, assign, append).
 *
 * @return 1 on success, 0 on failure (e.g., malloc error).
 */
static int	handle_env_ops(t_env *env_node, const char *token, t_export_op op)
{
	if (op == EXPORT_NONE)
		env_node->in_env = false;
	if (op == EXPORT_ASSIGN)
	{
		if (!update_existing_env_node(env_node, token))
			return (0);
	}
	else if (op == EXPORT_APPEND)
	{
		if (!append_env_value(env_node, token))
			return (0);
	}
	return (1);
}

/**
 * @brief Add or update an environment variable in the list.
 *
 * This function handles both creation of new environment nodes and updating
 * of existing ones based on the detected operation (assignment, append, or
 * none).
 *
 * Steps:
 *  - Detect the export operation type (assignment, append, or none).
 *  - Extract and validate the key from the given token.
 *  - If a node with the key already exists, update or append its value.
 *  - Otherwise, create and append a new environment node to the list.
 *
 * @param env_list Pointer to the environment list (linked list of t_env).
 * @param token    The input string containing the assignment or declaration.
 *
 * @return 1 on success
 *         0 on invalid key (error printed, but not fatal)
 *        -1 on malloc failure
 *
 * @note The @p key is freed internally after being used.
 */
int	set_env_node(t_list **env_list, const char *token)
{
	char		*key;
	t_env		*env_node;
	t_export_op	op;

	if (!token || !env_list)
		return (-1);
	op = detect_operation(token);
	key = get_env_key(token); // extract and validate key
	if (!key)
		return (handle_invalid_key(token));
	env_node = get_env_node_by_key(*env_list, key);
	if (env_node) // if we found existing key
	{
		free(key);
		if (!handle_env_ops(env_node, token, op))
			return (-1);
	}
	else // no key found so we create a new node
	{
		env_node = create_new_env_node(key, token, op);
		if (!env_node)
			return (-1);
		ft_lstadd_back(env_list, ft_lstnew(env_node));
	}
	return (1);
}

/**
 * @brief Implements the `export` builtin command.
 *
 * Handles the `export` builtin for displaying or updating environment
 * variables.
 *
 * Behavior:
 * - If called with no additional tokens (only "export"), prints the
 *   environment in sorted format (`declare -x` style).
 * - If called with key/value assignments or variable names:
 *   - Valid identifiers are added or updated in the environment list.
 *   - Invalid identifiers produce an error message but do not stop
 *     processing the remaining tokens.
 *
 * @param tokens Array of command arguments (first token is "export").
 * @param data   Pointer to shell data containing the environment list.
 *
 * @return int
 * - 0 on success (including if some identifiers were invalid).
 * - 1 if a fatal error occurs (e.g., malloc failure in `set_env_node`).
 */
int	builtin_export(char **tokens, t_shell *data)
{
	int		i;
	int		size;
	t_env	**env_array;

	size = 0;
	if (tokens[1] == NULL)
	{
		env_array = export_list_to_array(data->env_list, &size);
		if (!env_array)
			return (1);
		sort_export_array(env_array, size);
		print_sorted_export(env_array, size);
		free(env_array);
		return (0);
	}
	i = 1;
	while (tokens[i])
	{
		if (set_env_node(&data->env_list, tokens[i]) == -1)
			return (1);
		i++;
	}
	return (0);
}
