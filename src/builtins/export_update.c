#include "minishell.h"

/**
 * @brief Create a new environment variable node.
 *
 * Allocates and initializes a new t_env struct with the given key
 * and the value extracted from the token (if applicable).
 *
 * - If `op == EXPORT_ASSIGN`, the value is parsed from the token and
 *   stored in the node, and `in_env` is set to true.
 * - Otherwise, the node is created without a value and `in_env` is set to
 *   false.
 *
 * @param key   A malloc'ed key string (ownership is transferred to the node).
 * @param token The input string containing the assignment.
 * @param op    The detected export operation (assign, append, or none).
 *
 * @return Pointer to the newly created t_env node, or NULL on malloc failure.
 *         On failure, the provided key is freed.
 */
t_env	*create_new_env_node(char *key, const char *token, t_export_op op)
{
	t_env	*new;

	new = malloc(sizeof (t_env));
	if (!new)
		return (free(key), NULL);
	new->key = key; // already malloced in set_env_node
	if (op == EXPORT_ASSIGN)
	{
		new->value = get_env_value(token);
		if (!new->value)
		{
			free(new->key);
			free(new);
			return (NULL);
		}
		new->in_env = true;
	}
	else
	{
		new->value = NULL;
		new->in_env = false;
	}
	return (new);
}

/**
 * @brief Replace the value of an existing environment variable.
 *
 * Extracts the value from the given token and assigns it to the
 * environment node, freeing the previous value. The variable is
 * also marked as exported (`in_env = true`).
 *
 * @param env_node The environment node to update.
 * @param token    The input string containing the new assignment.
 *
 * @return 1 on success, 0 on failure.
 */
int	update_existing_env_node(t_env *env_node, const char *token)
{
	char	*new_value;

	if (!env_node || !token)
		return (0);
	new_value = get_env_value(token);
	if (!new_value)
	{
		errno = ENOMEM;
		return (0);
	}
	free(env_node->value); // free old value
	env_node->value = new_value;
	env_node->in_env = true;
	return (1);
}

/**
 * @brief Append a new value to an existing environment variable.
 *
 * This function handles the `VAR+=value` case in `export`. It retrieves the
 * value from the given token and concatenates it to the current value of the
 * environment variable. If the variable has no value yet, the new value is
 * simply assigned.
 *
 * @param env_node Pointer to the existing environment variable node.
 * @param token    The input token (e.g. "VAR+=value").
 *
 * @return 1 on success, 0 on failure (malloc error or invalid input).
 *
 * @note Frees and replaces the old `env_node->value` with the appended result.
 */
int	append_env_value(t_env *env_node, const char *token)
{
	char	*new_value;
	char	*old_value;
	char	*appended;

	if (!env_node || !token)
		return (0);
	new_value = get_env_value(token);
	if (!new_value)
		return (0);
	if (env_node->value)
		old_value = env_node->value;
	else
		old_value = "";
	appended = ft_strjoin(old_value, new_value);
	free(new_value);
	if (!appended)
		return (0);
	free(env_node->value);
	env_node->value = appended;
	env_node->in_env = true;
	return (1);
}
