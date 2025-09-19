#include "minishell.h"

/**
 * @brief Handles an invalid export key by printing an error.
 *
 * Checks if errno indicates a memory allocation failure (ENOMEM).
 * If so, returns -1 to signal a fatal error. Otherwise, prints
 * an error message for the invalid key and returns EXIT_FAILURE (1).
 *
 * @param token  The invalid token to report.
 *
 * @return int -1 if errno == ENOMEM, EXIT_FAILURE (1) otherwise.
 */
static int	handle_invalid_key(const char *token)
{
	if (errno == ENOMEM)
		return (-1);
	print_error(ERR_PREFIX, ERR_EXPORT, (char *)token, ERR_NOT_VALID_ID);
	return (EXIT_FAILURE);
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
	else if (op == EXPORT_ASSIGN)
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
	key = get_env_key(token);
	if (!key)
		return (handle_invalid_key(token));
	env_node = get_env_node_by_key(*env_list, key);
	if (env_node)
	{
		free(key);
		if (!handle_env_ops(env_node, token, op))
			return (-1);
	}
	else
	{
		env_node = create_new_env_node(key, token, op);
		if (!env_node)
			return (-1);
		ft_lstadd_back(env_list, ft_lstnew(env_node));
	}
	return (0);
}

/**
 * @brief Implements the `export` builtin.
 *
 * Without arguments, prints all environment variables in sorted order.
 * With arguments, sets or appends variables. Invalid keys are skipped,
 * and `data->status` is updated if any errors occur.
 *
 * @param tokens Array of command tokens, with `tokens[0]` being "export".
 * @param data   Pointer to shell state containing the environment and status.
 *
 * @return 0 on success, 1 on fatal errors (e.g., malloc failure).
 *         Invalid keys do not stop execution but set `data->status`.
 */
int	builtin_export(char **tokens, t_shell *data)
{
	int		i;
	int		size;
	t_env	**env_array;
	int		result;

	if (tokens[1] == NULL)
	{
		env_array = export_list_to_array(data->env_list, &size);
		if (!env_array)
			return (EXIT_FAILURE);
		sort_export_array(env_array, size);
		print_sorted_export(env_array, size);
		return (free(env_array), EXIT_SUCCESS);
	}
	i = 1;
	while (tokens[i])
	{
		result = set_env_node(&data->env_list, tokens[i]);
		if (result == -1)
			return (EXIT_FAILURE);
		if (result == 1)
			data->status = 1;
		i++;
	}
	return (data->status);
}
