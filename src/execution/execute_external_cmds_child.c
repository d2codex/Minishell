#include "minishell.h"

/**
 * @brief Execute command in child process.
 *
 * Restores default signal handlers so external commands react normally
 * to signals (e.g., Ctrl-C terminates, Ctrl-\ dumps core). Then replaces
 * the current process image with the specified command using execve().
 *
 * If execve() fails (for example, due to permission issues or missing binary),
 * prints an error message using perror(), cleans up allocated resources,
 * and exits with code 126 (CMD_NOT_EXECUTABLE).
 *
 * @param path   Full path to the executable
 * @param tokens Command arguments (argv for execve)
 * @param envp   Environment variables array
 *
 * @note This function does not return on success, because execve()
 *       replaces the current process image. It only returns via exit()
 *       if execve() fails.
 */
void	child_process(char *path, char **tokens, char **envp)
{
	// restore default signal handling so the command behaves like in bash in non interactive mode
	setup_signals_child();
	execve(path, tokens, envp);

	// if execve() returns, it failed = perform cleanup before exiting
	free(path);
	free_strings_array(envp);
	perror("execve");
	exit(CMD_NOT_EXECUTABLE);
}

/**
 * @brief Execute an external command directly within a child process.
 *
 * Used when the current process is already a forked child (ex: in a pipeline),
 * meaning that forking again would be redundant. This helper prepares
 * the environment and executable path, then calls child_process().
 *
 * If initialization fails (command not found, env allocation fails, etc.),
 * it exits immediately with the appropriate status code.
 *
 * @param tokens Command arguments (argv-style array)
 * @param data   Shell data structure containing environment info
 *
 * @note This function does not return: either calls execve() (via child_process)
 *       or exits with an error code if initialization fails.
 */
void	execute_in_child_mode(char **tokens, t_shell *data)
{
	char	*path;
	char	**envp;
	int		status;

	status = init_execution(tokens, data, &path, &envp);
	if (status != 0)
		exit(status);
	child_process(path, tokens, envp);
}


/**
 * @brief Waits for a child process to complete and handles signals properly.
 *
 * The parent process temporarily ignores interactive signals (SIGINT, SIGQUIT)
 * while waiting for the child. It repeatedly calls waitpid() until it returns
 * successfully, handling interruptions (EINTR) caused by signals.
 *
 * Once the child has finished:
 * - Restores normal signal handling for the shell
 * - Prints messages consistent with bash behavior when terminated by signals
 * - Frees allocated memory (path and envp)
 * - Returns the child’s exit code via parent_process()
 *
 * @param pid   Process ID of the child to wait for
 * @param path  Path string to free after completion
 * @param envp  Environment array to free after completion
 * @return Exit code of the finished child process, or EXIT_FAILURE on error
 */
int	wait_for_child(pid_t pid, char *path, char **envp)
{
	int	status;

	// Ignore interactive signals (Ctrl-C, Ctrl-\) while waiting
	setup_signals_ignore();
	while (waitpid(pid, &status, 0) == -1)
	{
		// retry waitpid() if interrupted by a signal (EINTR)
		if (errno != EINTR)
		{
			// on unexpected error, print error, free memory, and restore signals
			perror("waitpid");
			free(path);
			free_strings_array(envp);
			setup_signals_interactive();
			return (EXIT_FAILURE);
		}
	}
	// child finished: restore shell’s normal signal handling
	setup_signals_interactive();
	// if the child was killed by a signal, print corresponding message
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 20);
		else
			write(1, "\n", 1);
	}
	free(path);
	free_strings_array(envp);
	// return the normalized child exit code (like bash)
	return (parent_process(status));
}



