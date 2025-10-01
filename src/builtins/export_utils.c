#include "minishell.h"

/**
 * @brief Detects the type of export operation in a arg.
 *
 * Scans the input string to determine whether it represents:
 * - No operation (`EXPORT_NONE`) if there is no '=' or '+=' present.
 * - Assignment (`EXPORT_ASSIGN`) if there is a '=' not preceded by '+'.
 * - Append (`EXPORT_APPEND`) if there is a '+=' sequence.
 *
 * @param arg The input string to analyze.
 * @return The detected export operation as a t_export_op enum value.
 */
t_export_op	detect_operation(const char *arg)
{
	int	i;

	if (!arg)
		return (EXPORT_NONE);
	i = 0;
	while (arg[i] && arg[i] != '=' && arg[i] != '+')
		i++;
	if (arg[i] == '\0')
		return (EXPORT_NONE);
	if (arg[i] == '=')
		return (EXPORT_ASSIGN);
	if (arg[i] == '+' && arg[i + 1] && arg[i + 1] == '=')
		return (EXPORT_APPEND);
	return (EXPORT_ASSIGN);
}

/**
 * @brief Checks if a given arg is a valid shell environment variable key.
 *
 * A valid key must:
 *  - Not be NULL or empty
 *  - Start with a letter (a-z, A-Z) or underscore (_)
 *  - Contain only alphanumeric characters (a-z, A-Z, 0-9) or underscores (_)
 *	up to an optional '=' or '+' character.
 *
 * @param arg The input string representing the arg to validate.
 * @return true if the arg is a valid key according to the rules, false
 * otherwise.
 */
bool	is_valid_key(const char *arg)
{
	int	i;

	if (!arg || arg[0] == '\0')
		return (false);
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (false);
	i = 1;
	while (arg[i] && arg[i] != '=' && arg[i] != '+')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

/**
 * @brief Extracts the key part from an environment arg.
 *
 * Scans the input string until the first '=' or '+' character (for append
 * operations), returning a newly allocated string containing only the key.
 * If the arg is invalid, returns NULL. If memory allocation fails,
 * sets errno to ENOMEM and returns NULL, allowing the caller to distinguish
 * between invalid keys and malloc failure.*
 *
 * @param arg Input string in the form KEY=VALUE, KEY+=VALUE, or KEY.
 *
 * @return A malloc'ed copy of the key, or NULL if the key is invalid or on
 * malloc failure. Caller is responsible for freeing the returned string.
 */
char	*get_env_key(const char *arg)
{
	char	*key;
	int		len;

	if (!is_valid_key(arg))
		return (NULL);
	len = 0;
	while (arg[len] && arg[len] != '+' && arg[len] != '=')
		len++;
	key = malloc(len + 1);
	if (!key)
	{
		errno = ENOMEM;
		return (NULL);
	}
	ft_strlcpy(key, arg, len + 1);
	return (key);
}

/**
 * @brief Extract the value part from a arg (after '=').
 *
 * @param arg Input string in the form KEY=VALUE.
 *
 * @return A malloc'ed copy of the value, or NULL if no '=' found or malloc
 * fails.
 *		 Caller must free the returned string.
 */
/* char	*get_env_value(const char *arg)
{
	char	*value;
	char	*equal;

	if (!arg)
		return (NULL);
	equal = ft_strchr(arg, '=');
	if (!equal)
		return (NULL);
	value = ft_strdup(equal + 1);
	if (!value)
		return (NULL);
	return (value);
} */
char *get_env_value(const char *arg)
{
	char *value;
	char *equal;
	char *trimmed;

	if (!arg)
		return (NULL);
	equal = ft_strchr(arg, '=');
	if (!equal)
		return (NULL);
	value = ft_strdup(equal + 1);
	if (!value)
		return (NULL);
	trimmed = trim_quotes(value);
	free(value);
	return (trimmed);
}

/**
 * @brief Find an environment node by key in the list.
 *
 * @param env_list The linked list of environment variables.
 * @param key The key to search for.
 *
 * @return Pointer to the matching t_env node, or NULL if not found.
 */
t_env	*get_env_node_by_key(t_list *env_list, const char *key)
{
	t_env	*env_node;

	if (!env_list || !key)
		return (NULL);
	while (env_list)
	{
		env_node = env_list->content;
		if (env_node && ft_strcmp(env_node->key, key) == 0)
			return (env_node);
		env_list = env_list->next;
	}
	return (NULL);
}
