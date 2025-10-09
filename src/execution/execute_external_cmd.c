#include "minishell.h"

/**
 * @brief Initialize execution by finding the executable and preparing envp.
 *
 * Locates the command in PATH (or verifies absolute path) and converts
 * the environment list into an array suitable for execve(). If any step
 * fails, cleans up allocated resources and returns an error code.
 *
 * @param tokens Command arguments (tokens[0] is the command name)
 * @param data Shell data structure containing environment
 * @param path Output parameter for the executable path
 * @param envp Output parameter for the environment array
 * @return 0 on success, CMD_NOT_FOUND (127) if command not found,
 *	 EXIT_FAILURE (1) if env conversion fails
 */
static int	init_execution(char **tokens, t_shell *data, char **path,
	char ***envp)
{
	*path = find_executable(tokens[0], data);
	if (!*path)
	{
		print_error(tokens[0], ERR_CMD_NOT_FOUND, NULL, NULL);
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
 * @brief Execute command in child process.
 *
 * Marks the shell as running in a child process, then replaces
 * the current process image with the specified command using execve().
 * If execve() fails (ex: permission denied), prints an error and
 * exits with code 126.
 *
 * @param path Full path to the executable
 * @param tokens Command arguments (argv for execve)
 * @param envp Environment variables array
 *
 * @note This function does not return on success (process is replaced).
 *		 Only returns via exit() if execve() fails.
 */
static void	child_process(char *path, char **tokens, char **envp)
{
	execve(path, tokens, envp);
	// execve failed - cleanup before exit
	free(path);
	free_strings_array(envp);
	perror("execve");
	exit(CMD_NOT_EXECUTABLE);
}

/**
 * @brief Handle fork() failure by cleaning up resources.
 *
 * @param path Allocated path string to free
 * @param envp Allocated environment array to free
 * @return EXIT_FAILURE (1)
 */
static int	handle_fork_error(char *path, char **envp)
{
	perror("fork");
	free(path);
	free_strings_array(envp);
	return (EXIT_FAILURE);
}

/**
 * @brief Extract and return the exit status from a child process.
 *
 * Analyzes the status value returned by wait() to determine how
 * the child process terminated and extracts the appropriate exit code.
 *
 * @param status Status value from wait() or waitpid()
 * @return Exit code of the child process if it terminated normally,
 *	 EXIT_FAILURE (1) if terminated abnormally
 */
static int	parent_process(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}

/**
 * @brief Execute an external command (non-builtin).
 *
 * Finds the command in PATH, forks a child process, and executes
 * the command using execve(). The parent process waits for the child
 * to complete and returns its exit status.
 *
 * If already running in a child process (from a pipeline), skips
 * forking and executes directly.
 *
 * @param tokens Command arguments where tokens[0] is the command name
 * @param data Shell data structure
 * @return Exit status of the executed command
 */
int	execute_external_command(char **tokens, t_shell *data)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**envp;
	int		ret;

	// Find executable path and prepare environment array for execve
	ret = init_execution(tokens, data, &path, &envp);
	if (ret != 0)
		return (ret);

	// Already in child (from pipeline): execute directly without forking again
	if (data->is_child)
		child_process(path, tokens, envp);  // Never returns (execve or exit)

	// Normal case: fork a new child process
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(path, envp));

	// Child: replace process with command
	if (pid == 0)
		child_process(path, tokens, envp);  // Never returns

	// Parent: wait for child to complete
	wait(&status);

	// Cleanup allocated resources
	free(path);
	free_strings_array(envp);

	// Extract and return exit code from child
	return (parent_process(status));
}
