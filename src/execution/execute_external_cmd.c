/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:41:00 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/15 17:41:01 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialize execution by finding the executable and preparing envp.
 *
 * Locates the command in PATH (or verifies absolute path) and converts
 * the environment list into an array suitable for execve(). If any step
 * fails, cleans up allocated resources and returns an error code.
 *
 * @param argv Command arguments (argv[0] is the command name)
 * @param data Shell data structure containing environment
 * @param path Output parameter for the executable path
 * @param envp Output parameter for the environment array
 * @return 0 on success, CMD_NOT_FOUND (127) if command not found,
 *	 EXIT_FAILURE (1) if env conversion fails
 */
static int	init_execution(char **argv, t_shell *data, char **path,
	char ***envp)
{
	*path = find_executable(argv[0], data);
	if (!*path)
	{
		if (ft_strchr(argv[0], '/'))
			print_error(argv[0], ERR_NO_SUCH_FILE, NULL, NULL);
		else
			print_error(argv[0], ERR_CMD_NOT_FOUND, NULL, NULL);
		return (CMD_NOT_FOUND);
	}
	*envp = env_list_to_array(data->env_list);
	if (!*envp)
	{
		free(*path);
		return (EXIT_FAILURE);
	}
	return (0);
}

/**
 * @brief Execute an external command in the current (child) process.
 *
 * This function prepares the executable path and environment array, then
 * replaces the current process image with the external program using `execve()`
 * It is intended to be called only in a forked child process.
 *
 * @param argv Argument vector for the command (NULL-terminated).
 * @param data   Pointer to the main shell structure containing environment data
 *
 * @note This function never returns. If `execve()` fails, it cleans up
 * and exits
 *       with `CMD_NOT_EXECUTABLE`. The parent process should handle the
 * exit code
 *       via `waitpid()`.
 */
int	execute_external_command(char **argv, t_shell *data)
{
	char	*path;
	char	**envp;
	int		init_status;

	init_status = init_execution(argv, data, &path, &envp);
	if (init_status != 0)
		exit (init_status);
	if (data->curr_ast)
		close_all_heredocs(data->curr_ast);
	execve(path, argv, envp);
	perror("execve");
	free(path);
	free_strings_array(envp);
	exit(CMD_NOT_EXECUTABLE);
}
