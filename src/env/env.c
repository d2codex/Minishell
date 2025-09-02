#include "minishell.h"

/**
 * @brief Free an environment variable struct.
 *
 * @param content Pointer to a t_env struct to free.
 *
 * @note This function is used as a callback for ft_lstclear, 
 *       which expects a function with a void pointer parameter.
 */
void	del_env(void *content)
{
	t_env	*env;

	env = (t_env *)content;
	if (!env)
		return ;
	free(env->key);
	free(env->value);
	free(env);
}

/**
 * @brief Print all environment variables in the list.
 *
 * @param env_list Pointer to the head of the environment list.
 *
 * @note Format is KEY=VALUE per line.  
 *       If a variable has an empty value, it prints KEY= with nothing after
 *       the '='. Skips NULL nodes.
 */
void	print_env_list(t_list *env_list)
{
	t_list	*current;
	t_env	*env;

	current = env_list;
	while (current)
	{
		env = (t_env *)current->content;
		if (env)
			printf("%s=%s\n", env->key, env->value);
		current = current->next;
	}
}

/**
 * @brief Create a new environment node from a "KEY=VALUE" string.
 *
 * @param str Input string in the form "KEY=VALUE".
 * @return Pointer to allocated t_env struct, or NULL if:
 *         - Allocation fails
 *         - str is NULL
 *         - No '=' found
 *         - Key is empty
 *
 * @note Value may be empty. Invalid variables are ignored in the caller.
 */
t_env	*create_env_node(const char *str)
{
	t_env	*env;
	char	*eq;

	if (!str)
		return (NULL);
	eq = ft_strchr(str, '=');
	if (!eq || eq == str)
		return (NULL);
	env = malloc(sizeof (t_env));
	if (!env)
		return (NULL);
	env->key = ft_substr(str, 0, eq - str);
	if (!env->key)
	{
		free(env);
		return (NULL);
	}
	env->value = ft_strdup(eq + 1);
	if (!env->value)
	{
		free(env->key);
		free(env);
		return (NULL);
	}
	return (env);
}

/**
 * @brief Initialize a linked list of environment variables from envp.
 *
 * @param envp Null-terminated array of strings in the form "KEY=VALUE".
 * @return Pointer to the head of a newly allocated t_list containing t_env
 *         structs. Returns NULL if envp is NULL or empty.
 *
 * @note Each t_env is created with create_env_node(). Invalid variables
 *       (NULL, missing '=', empty key) are skipped. Memory for the list
 *       must be freed by the caller with ft_lstclear() and del_env().
 */
t_list	*init_env_from_envp(char **envp)
{
	t_list	*env_list;
	t_list	*node;
	t_env	*env;
	int		i;

	env_list = NULL;
	if (!envp)
		return (NULL);
	i = 0;
	while (envp[i] != NULL)
	{
		env = create_env_node(envp[i]);
		if (env)
		{
			node = ft_lstnew(env);
			if (node)
				ft_lstadd_back(&env_list, node);
			else
				del_env(env);
		}
		i++;
	}
	return (env_list);
}
