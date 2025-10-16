/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env_array.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:40:45 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/15 17:40:45 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Count how many exported variables must appear in envp.
 *
 * Iterates through the environment list and counts all variables
 * marked as exported (in_env == true). These are the variables
 * that will be passed to execve().
 *
 * @param env_list Linked list of environment variables
 * @return Number of exported variables
 */
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

/**
 * @brief Build a string in the format "KEY=value".
 *
 * Creates a new string by concatenating the environment variable's
 * key, an equals sign, and its value. The result is suitable for
 * use in the envp array passed to execve().
 *
 * @param env Environment variable node containing key and value
 * @return Newly allocated string "KEY=value", or NULL on malloc failure
 *
 * @note Caller is responsible for freeing the returned string.
 */
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

/**
 * @brief Add one variable to env_array and handle malloc failure.
 *
 * Creates the "KEY=value" string for the given environment variable
 * and stores it in the array at the specified index. If string creation
 * fails, frees the entire array to prevent memory leaks.
 *
 * @param env_array Array being built for envp
 * @param i Pointer to current index in the array (incremented on success)
 * @param env Environment variable to add
 * @return 0 on success, 1 on malloc failure
 */
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

/**
 * @brief Convert environment list to char** array for execve().
 *
 * Transforms the linked list of environment variables into a
 * NULL-terminated array of strings in "KEY=value" format, suitable
 * for passing to execve(). Only variables marked as exported
 * (in_env == true) are included.
 *
 * @param env_list Linked list of environment variables
 * @return NULL-terminated array of "KEY=value" strings, or NULL on error
 *
 * @note Caller must free the returned array using free_strings_array().
 */
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
