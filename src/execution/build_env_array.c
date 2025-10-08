#include "minishell.h"

//TODO: Doxygen coms

/* Count how many exported vars must appear in envp */
static size_t	count_exported_vars(t_list *env_list)
{
	size_t	count;
	t_list	*current;
	t_env	*env;

	count = 0;
	current = env_list;
	while (current)
	{
		env = (t_env *)current->content;
		if (env->in_env)
			count++;
		current = current->next;
	}
	return (count);
}

/* Build a string like "KEY=value" */
static char	*create_env_string(t_env *env)
{
	char	*result;
	size_t	key_len;
	size_t	value_len;

	if (!env || !env->key || !env->value)
		return (NULL);
	key_len = ft_strlen(env->key);
	value_len = ft_strlen(env->value);
	result = malloc(sizeof(char) * (key_len + value_len + 2));
	if (!result)
		return (NULL);
	ft_strlcpy(result, env->key, key_len + 1);
	result[key_len] = '=';
	ft_strlcpy(result + key_len + 1, env->value, value_len + 1);
	return (result);
}

/* Add one variable to env_array, handle malloc failure */
static int	add_env_to_array(char **env_array, size_t *i, t_env *env)
{
	env_array[*i] = create_env_string(env);
	if (!env_array[*i])
	{
		free_strings_array(env_array);
		return (1);
	}
	(*i)++;
	return (0);
}

/* Convert env_list to char ** for execve() */
char	**env_list_to_array(t_list *env_list)
{
	char	**env_array;
	size_t	count;
	size_t	i;
	t_list	*current;
	t_env	*env;

	if (!env_list)
		return (NULL);
	count = count_exported_vars(env_list);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	current = env_list;
	while (current)
	{
		env = (t_env *)current->content;
		if (env->in_env && add_env_to_array(env_array, &i, env))
			return (NULL);
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
