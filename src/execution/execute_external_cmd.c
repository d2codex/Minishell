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
 * replaces the current process image with the external program using `execve()`.
 * It is intended to be called only in a forked child process.
 *
 * @param argv Argument vector for the command (NULL-terminated).
 * @param data   Pointer to the main shell structure containing environment data.
 *
 * @note This function never returns. If `execve()` fails, it cleans up and exits
 *       with `CMD_NOT_EXECUTABLE`. The parent process should handle the exit code
 *       via `waitpid()`.
 */
int	execute_external_command(char **argv, t_shell *data)
{
	char	*path;
	char	**envp;
	int		init_status;

	// Find executable path and prepare environment array for execve
	init_status = init_execution(argv, data, &path, &envp);
	if (init_status != 0)
		exit (init_status);
	// Try to execute the external command
	execve(path, argv, envp);
	// If execve returns, it failed, so clean up before exiting
	perror("execve");
	free(path);
	free_strings_array(envp);
	exit(CMD_NOT_EXECUTABLE);
}
