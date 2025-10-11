#include "minishell.h"

/**
 * @brief Initialize execution by locating the executable and preparing envp.
 *
 * Finds the absolute path to the command (via PATH lookup or direct access)
 * and converts the shell’s environment list into an array usable by execve().
 *
 * If the command cannot be found or memory allocation fails, cleans up
 * and returns an appropriate error code.
 *
 * @param tokens Command arguments (tokens[0] is the command name)
 * @param data   Shell state containing environment information
 * @param path   Output parameter for the resolved executable path
 * @param envp   Output parameter for the environment array
 * @return 0 on success,
 *         CMD_NOT_FOUND (127) if the command does not exist,
 *         EXIT_FAILURE (1) if environment conversion fails
 *         EXIT_SUCCESS (0) if ok
 */
int	init_execution(char **tokens, t_shell *data, char **path,
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
	return (EXIT_SUCCESS);
}

/**
 * @brief Handle fork() failure by cleaning up allocated resources.
 *
 * Called when fork() fails during command execution. It frees memory
 * associated with the executable path and environment, prints an error
 * message, and returns EXIT_FAILURE.
 *
 * @param path Allocated executable path to free
 * @param envp Allocated environment array to free
 * @return EXIT_FAILURE (1)
 */
int	handle_fork_error(char *path, char **envp)
{
	perror("fork");
	free(path);
	free_strings_array(envp);
	return (EXIT_FAILURE);
}

/**
 * @brief Extract the final exit status from a child process.
 *
 * Interprets the status value returned by waitpid() to
 * determine how the child terminated:
 * - If exited normally: returns the exit code
 * - If terminated by a signal: returns 128 + signal number
 * - Otherwise: returns EXIT_FAILURE
 *
 * Matches standard bash exit code conventions.
 *
 * @param status Status value returned by wait() or waitpid()
 * @return Normalized exit code of the child process
 */
int	parent_process(int status)
{
	// normal termination: return the child’s exit code
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));

	// termination by signal: follow bash convention (128 + signal)
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));

	// default fallback
	return (EXIT_FAILURE);
}

/**
 * @brief Execute an external (non-builtin) command.
 *
 * Handles the full lifecycle of an external command:
 * - Resolves executable path and prepares environment
 * - Forks a new process
 * - Executes the command in the child
 * - Waits for completion in the parent
 *
 * If running inside an already-forked child (pipeline mode),
 * directly executes without forking again.
 *
 * @param tokens Command arguments (argv-style array)
 * @param data   Shell state containing environment and flags
 * @return Exit code of the executed command, or error code if it fails
 */
int	execute_external_command(char **tokens, t_shell *data)
{
	pid_t	pid;
	char	*path;
	char	**envp;
	int		status;

	// if already inside a child (e.g., in a pipeline), execute directly
	if (data->is_child)
		execute_in_child_mode(tokens, data);

	// prepare path and environment for execve()
	status = init_execution(tokens, data, &path, &envp);
	if (status != 0)
		return (status);

	// create a new process to run the external command
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(path, envp));

	// child process: execute the command (never returns if successful)
	if (pid == 0)
		child_process(path, tokens, envp);

	// parent process: wait for child completion and handle signals
	return (wait_for_child(pid, path, envp));
}
