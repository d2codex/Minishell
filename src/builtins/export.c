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
int	handle_invalid_key(const char *token)
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
 * @brief Create and initialize a new env node.
 *
 * Takes ownership of `key` and builds a `t_env` from `token`.
 * If `token` has '=', a value is stored and `in_env` is true.
 * Otherwise value is NULL and `in_env` is false.
 *
 * @param key   Allocated key (freed on failure).
 * @param token String like "KEY=value" or "KEY".
 *
 * @return New env node, or NULL on allocation failure.
 */
t_env	*create_new_env_node(char *key, const char *token)
{
	t_env	*new_node;
	char	*value;

	value = NULL;
	new_node = malloc(sizeof (t_env));
	if (!new_node)
		return (free(key), NULL);
	new_node->key = key;
	if (has_equal(token))
	{
		value = get_env_value(token);
		if (!value)
			return (free(key), free(new_node), NULL);
		new_node->value = value;
		new_node->in_env = true;
	}
	else
	{
		new_node->value = NULL;
		new_node->in_env = false;
	}
	return (new_node);
}

/**
 * @brief Update an existing env node with a new token.
 *
 * If `token` has '=', the node's value is replaced and `in_env` is set true.
 * If no '=', only `in_env` is set false. Old value is freed if replaced.
 *
 * @param env_node Target node to update.
 * @param token    String like "KEY=value" or "KEY".
 *
 * @return 1 on success, 0 on malloc failure or invalid args.
 */
int	update_existing_env_node(t_env *env_node, const char *token)
{
	char	*new_value;

	if (!env_node || !token)
		return (0);
	if (!has_equal(token))
	{
		// if no '=' will not be in env table
		env_node->in_env = false;
		return (1);
	}
	new_value = get_env_value(token);
	if (!new_value)
		return (0); //malloc failure free key set_env_node()
	free(env_node->value); // free old value
	env_node->value = new_value;
	env_node->in_env = true;
	return (1);
}

/**
 * @brief Adds or updates an environment variable node in the list.
 *
 * Given a token in the form of `KEY=value` or `KEY`, this function:
 * - Validates and extracts the key.
 * - If the key exists in the environment list, updates the corresponding node.
 * - If the key does not exist, creates and appends a new node.
 * - If the token is invalid, an error is printed and the operation is skipped.
 *
 * @param env_list Pointer to the environment list (linked list of t_env).
 * @param token    The string token to parse (e.g., "FOO=bar").
 *
 * @return int
 * - 1 on success (node created or updated).
 * - 0 on invalid identifier (error printed, but not fatal).
 * - -1 on fatal error (malloc failure or invalid arguments).
 */
int	set_env_node(t_list **env_list, const char *token)
{
	char	*key;
	t_env	*env_node;

	if (!token || !env_list)
		return (-1);
	key = get_env_key(token); // extract and validate key
	if (!key)
		return (handle_invalid_key(token));
	env_node = get_env_node_by_key(*env_list, key);
	if (env_node)
	{
		free(key);
		if (!update_existing_env_node(env_node, token))
			return (-1);
	}
	else // no key found so we create a new node
	{
		env_node = create_new_env_node(key, token);
		if (!env_node)
			return (-1);
		ft_lstadd_back(env_list, ft_lstnew(env_node));
	}
	return (1);
}


/**
 * @brief Implements the `export` builtin command.
 *
 * This function handles the `export` builtin for updating or displaying
 * environment variables.
 *
 * Behavior:
 * - If called with no additional tokens (only "export"), it prints the
 *   environment in sorted format (`declare -x` style).
 * - If called with key/value assignments or variable names, it validates
 *   and updates the environment list:
 *   - Valid identifiers are added or updated.
 *   - Invalid identifiers are rejected with an error message, but execution
 *     continues for the remaining tokens.
 *
 * @param env_list Pointer to the environment list (linked list of t_env).
 * @param tokens   Array of command arguments (first token is "export").
 *
 * @return int
 * - 0 on success (even if some identifiers were invalid).
 * - 1 if a fatal error occurs (e.g., malloc failure).
 */
int	builtin_export(t_list **env_list, char **tokens)
{
	int		i;
	int		size;
	int		status;
	t_env	**env_array;

	size = 0;
	if (tokens[1] == NULL)
	{
		env_array = export_list_to_array(*env_list, &size);
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
		status = set_env_node(env_list, tokens[i]);
		if (status == -1)
			return (1);
		i++;
	}
	return (0);
}
