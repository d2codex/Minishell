/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:40:02 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/15 17:40:09 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles an invalid export key by printing an error.
 *
 * Checks if errno indicates a memory allocation failure (ENOMEM).
 * If so, returns -1 to signal a fatal error. Otherwise, prints
 * an error message for the invalid key and returns EXIT_FAILURE (1).
 *
 * @param arg  The invalid arg to report.
 *
 * @return int -1 if errno == ENOMEM, EXIT_FAILURE (1) otherwise.
 */
static int	handle_invalid_key(const char *arg)
{
	if (errno == ENOMEM)
		return (-1);
	print_error(ERR_PREFIX, ERR_EXPORT, (char *)arg, ERR_NOT_VALID_ID);
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
 * @param arg    The input string containing the assignment or declaration.
 * @param op       The detected operation type (none, assign, append).
 *
 * @return 1 on success, 0 on failure (e.g., malloc error).
 */
static int	handle_env_ops(t_env *env_node, const char *arg, t_export_op op)
{
	if (op == EXPORT_NONE)
		env_node->in_env = false;
	else if (op == EXPORT_ASSIGN)
	{
		if (!update_existing_env_node(env_node, arg))
			return (0);
	}
	else if (op == EXPORT_APPEND)
	{
		if (!append_env_value(env_node, arg))
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
 *  - Extract and validate the key from the given arg.
 *  - If a node with the key already exists, update or append its value.
 *  - Otherwise, create and append a new environment node to the list.
 *
 * @param env_list Pointer to the environment list (linked list of t_env).
 * @param arg    The input string containing the assignment or declaration.
 *
 * @return 1 on success
 *         0 on invalid key (error printed, but not fatal)
 *        -1 on malloc failure
 *
 * @note The @p key is freed internally after being used.
 */
int	set_env_node(t_list **env_list, const char *arg)
{
	char		*key;
	t_env		*env_node;
	t_export_op	op;

	if (!arg || !env_list)
		return (-1);
	op = detect_operation(arg);
	key = get_env_key(arg);
	if (!key)
		return (handle_invalid_key(arg));
	env_node = get_env_node_by_key(*env_list, key);
	if (env_node)
	{
		free(key);
		if (!handle_env_ops(env_node, arg, op))
			return (-1);
	}
	else
	{
		env_node = create_new_env_node(key, arg, op);
		if (!env_node)
			return (-1);
		ft_lstadd_back(env_list, ft_lstnew(env_node));
	}
	return (0);
}

/**
 * @brief Convert the environment list to a sorted array and print it.
 *
 * This helper function handles the "export" command with no arguments.
 * It converts the linked list of environment variables to an array,
 * sorts it lexicographically, prints the result, and frees the array.
 * Updates data->status accordingly.
 *
 * @param env_list Linked list of environment variables
 * @param data Shell data structure
 * @return int Updated exit status (EXIT_SUCCESS or EXIT_FAILURE)
 */
static int	print_sorted_env(t_list *env_list, t_shell *data)
{
	int		size;
	t_env	**env_array;

	env_array = export_list_to_array(env_list, &size);
	if (!env_array)
	{
		data->status = EXIT_FAILURE;
		return (data->status);
	}
	sort_export_array(env_array, size);
	print_sorted_export(env_array, size);
	free(env_array);
	data->status = EXIT_SUCCESS;
	return (data->status);
}

/**
 * @brief Implements the export builtin command.
 *
 * Without arguments, prints all environment variables in sorted order.
 * With arguments, sets/updates the environment variables.
 * Updates data->status according to success or failure.
 *
 * @param argv Command argv array, where argv[0] is "export"
 * @param data Shell data structure
 * @return int Exit status of the last processed arg
 */
int	builtin_export(char **argv, t_shell *data)
{
	int		i;
	int		result;

	if (argv[1] == NULL)
		return (print_sorted_env(data->env_list, data));
	i = 1;
	while (argv[i])
	{
		result = set_env_node(&data->env_list, argv[i]);
		if (result == -1)
		{
			data->status = EXIT_FAILURE;
			return (data->status);
		}
		if (result == 1)
			data->status = 1;
		i++;
	}
	return (data->status);
}
