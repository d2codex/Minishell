#include "minishell.h"

void	print_export_error(const char *token)
{
	if (errno == EINVAL)
	{
		write(2, "$Hell: export: `", 20);
		write(2, token, ft_strlen(token));
		write(2, "': not a valid identifier\n", 26);
	}
}

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

int	update_existing_env_node(t_env *env_node, const char *token)
{
	char	*new_value;

	if (!env_node || !token)
		return (0);
	if (!has_equal(token))
	{
		env_node->in_env = false;
		return (1);
	}
	new_value = get_env_value(token);
	if (!new_value)
		return (0); //malloc failure free key on the main function?
	free(env_node->value); // free old value
	env_node->value = new_value;
	env_node->in_env = true;
	return (1);
}

int	set_env_node(t_list **env_list, const char *token)
{
	char	*key;
	t_env	*env_node;

	if (!token || !env_list)
		return (0);
	key = get_env_key(token); // extract and validate key
	if (!key)
		return (print_export_error(token), 0);
	env_node = get_env_node_by_key(*env_list, key);
	if (env_node)
	{
		free(key);
		if (!update_existing_env_node(env_node, token))
			return (0);
	}
	else
	{
		env_node = create_new_env_node(key, token);
		if (!env_node)
			return (0);
		ft_lstadd_back(env_list, ft_lstnew(env_node));
	}
	return (1);
}
