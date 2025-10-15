/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:40:13 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/15 17:40:13 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees an environment variable node and its list node.
 *
 * Frees the key and value strings, the t_env struct, and the associated
 * t_list node. Does nothing if either pointer is NULL.
 *
 * @param env_node   Pointer to the environment variable struct to free.
 * @param list_node  Pointer to the list node containing env_node.
 */
static void	free_env_node(t_env *env_node, t_list *list_node)
{
	if (!env_node || !list_node)
		return ;
	free(env_node->key);
	free(env_node->value);
	free(env_node);
	free(list_node);
}

/**
 * @brief Removes an environment variable node from the linked list by key.
 *
 * Searches the env_list for a node whose key matches the given string.
 * If found, the node is unlinked from the list and freed using free_env_node().
 * Handles removal of the head node as a special case.
 *
 * @param env_list  Pointer to the head of the environment list.
 * @param key       Key string of the environment variable to remove.
 *
 * @return int 0 if a node was found and removed,
 *             1 if the key was not found or list/env_list/key is NULL.
 */
int	remove_env_node(t_list **env_list, const char *arg)
{
	t_list	*prev;
	t_list	*curr;
	t_env	*env;

	prev = NULL;
	curr = *env_list;
	if (!env_list || !*env_list || !arg)
		return (1);
	while (curr)
	{
		env = (t_env *)curr->content;
		if (env && env->key && ft_strcmp(env->key, arg) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env_list = curr->next;
			free_env_node(env, curr);
			return (0);
		}
		prev = curr;
		curr = curr->next;
	}
	return (1);
}

/**
 * @brief Builtin command: remove environment variables.
 *
 * Removes the specified environment variables from `data->env_list`.
 * Ignores argv containing '='. Updates `data->status` with EXIT_SUCCESS
 * on success, or INTERNAL_ERROR if input is invalid.
 *
 * @param argv Command argv from user input.
 *               - argv[0] should be "unset"
 *               - argv[1+] are names of variables to remove
 * @param data Shell state, including environment and exit status.
 * @return Exit status of the command (0 on success, >0 on failure).
 */
int	builtin_unset(char **argv, t_shell *data)
{
	int	i;

	if (!argv || !data)
	{
		data->status = INTERNAL_ERROR;
		return (data->status);
	}
	i = 1;
	while (argv[i])
	{
		if (ft_strchr(argv[i], '='))
		{
			i++;
			continue ;
		}
		remove_env_node(&data->env_list, argv[i]);
		i++;
	}
	data->status = EXIT_SUCCESS;
	return (data->status);
}
