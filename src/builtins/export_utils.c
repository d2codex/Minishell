#include "minishell.h"

bool	has_equal(const char *token)
{
	if (ft_strchr(token, '=') != NULL)
		return (true);
	return (false);
}

/**
 * @brief Checks if a given token is a valid shell environment variable key.
 *
 * A valid key must:
 *  - Not be NULL or empty
 *  - Start with a letter (a-z, A-Z) or underscore (_)
 *  - Contain only alphanumeric characters (a-z, A-Z, 0-9) or underscores (_) 
 *    up to an optional '=' character
 *
 * @param token The input string representing the token to validate.
 * @return true if the token is a valid key according to the rules, false
 * otherwise.
 */
bool	is_valid_key(const char *token)
{
	int	i;

	// if token is null or empty
	if (!token || token[0] == '\0')
		return (false);
	// first character can only be letters or '_'
	// therefore the '=' check is included here if its the first char
	if (!ft_isalpha(token[0]) && token[0] != '_')
		return (false);
	// remaining can be alphanumeric or '_'
	i = 1;
	while (token[i] && token[i] != '=')
	{
		if (!ft_isalnum(token[i]) && token[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

// extract the key from the token
// remember to FREE it when done
char	*get_env_key(const char *token)
{
	char	*key;
	int		len;

	if (!is_valid_key(token))
	{
		errno = EINVAL; // to disginguis between malloc and input error
		return (NULL);
	}
	len = 0;
	while (token[len] && token[len] != '=')
		len++;
	key = malloc(len + 1);
	if (!key) //malloc failure, errno is already set to ENOMEM
		return (NULL);
	ft_strlcpy(key, token, len + 1);
	return (key);
}

char	*get_env_value(const char *token)
{
	char	*value;
	char	*equal;

	equal = ft_strchr(token, '=');
	if (!equal)
		return (NULL);
	value = ft_strdup(equal + 1);
	if (!value)
		return (NULL);
	return (value);
}

// check if the key exists in the env table
// return the node so it can be updated later
t_env	*get_env_node_by_key(t_list *env_list, const char *key)
{
	t_list	*current;
	t_env	*env_node;
	int		key_len;

	if (!env_list || !key)
		return (NULL);
	// need this for ft_strncmp (since strcmp is not in libft)
	key_len = ft_strlen(key);
	current = env_list;
	while (current)
	{
		// we need to cast the t_list to t_env since t_list is void arg
		env_node = (t_env *)current->content;
		// if env_node, just a defensive check making sure
		// current->content isn't NULL
		if (env_node && ft_strncmp(env_node->key, key, key_len) == 0
			&& ((int)ft_strlen(env_node->key) == key_len))
			return (env_node);
		current = current->next;
	}
	return (NULL);
}
