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
 *         EXIT_FAILURE (1) if env conversion fails
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
 * @param data Shell data structure
 *
 * @note This function does not return on success (process is replaced).
 *       Only returns via exit() if execve() fails.
 */
static void	child_process(char *path, char **tokens, char **envp, t_shell *data)
{
	data->is_child = true;
	execve(path, tokens, envp);
	perror("execve");
	exit(CMD_NOT_EXECUTABLE);
}

/**
 * @brief Extract and return the exit status from a child process.
 *
 * Analyzes the status value returned by wait() to determine how
 * the child process terminated and extracts the appropriate exit code.
 *
 * @param status Status value from wait() or waitpid()
 * @return Exit code of the child process if it terminated normally,
 *         EXIT_FAILURE (1) if terminated abnormally
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
 * Process:
 * 1. Find executable path and prepare environment
 * 2. Fork a child process
 * 3. Child: execute the command with execve()
 * 4. Parent: wait for child and collect exit status
 * 5. Clean up allocated resources
 *
 * @param tokens Command arguments where tokens[0] is the command name
 * @param data Shell data structure
 * @return Exit status of the executed command:
 *         - 0-125: command's exit status
 *         - 126: command found but not executable
 *         - 127: command not found
 *         - 1: fork or other system error
 */
int	execute_external_command(char **tokens, t_shell *data)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**envp;
	int		ret;

	ret = init_execution(tokens, data, &path, &envp);
	if (ret != 0)
		return (ret);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(path);
		free_strings_array(envp);
		return (EXIT_FAILURE);
	}
	if (pid == 0)
		child_process(path, tokens, envp, data);
	wait(&status);
	free(path);
	free_strings_array(envp);
	return (parent_process(status));
}

/**
 * @brief Execute an AST node (command or pipeline).
 *
 * Recursively executes the abstract syntax tree built from the parsed
 * command line. Handles both simple commands and pipelines (TODO).
 * For simple commands, checks if it's a builtin first, otherwise
 * executes as an external command.
 *
 * @param node AST node to execute (NODE_CMD or NODE_PIPE)
 * @param data Shell data structure
 * @return Exit status of the executed command
 *
 * @note Redirections (node->right) are not yet implemented (TODO).
 * @note Pipeline execution is not yet implemented (TODO).
 */
int	execute_ast_tree(t_ast *node, t_shell *data)
{
	if (!node)
		return (EXIT_SUCCESS);
	if (node->type == NODE_PIPE)
	{
		// TODO: implement pipeline execution (fork, pipe, exec left and right)
		printf("TODO: Execute pipeline\n");
		return (EXIT_SUCCESS);
	}
	else if (node->type == NODE_CMD)
	{
		// TODO: Handle redirections in node->right first
		if (execute_builtin(node, data))
			return (data->status);
		//TODO: execute external command using node->argv
		return (execute_external_command(node->argv, data));
	}
	return (EXIT_SUCCESS);
}
