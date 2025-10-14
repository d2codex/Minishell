#include "minishell.h"

/**
 * @brief Execute the left side of a pipeline in a child process.
 *
 * This function runs the left command of a pipe (e.g., in `cmd1 | cmd2`),
 * redirecting its standard output to the write end of the pipe so that
 * the right command can read it. It closes unused file descriptors and
 * ensures proper exit with the child’s execution status.
 *
 * @param node Pointer to the current AST node representing the pipeline.
 * @param data Pointer to the shell state structure.
 * @param pipefd The pipe file descriptors; pipefd[0] is the read end,
 *               and pipefd[1] is the write end.
 *
 * @details
 * Steps performed:
 * 1. Mark the shell as a child process (`data->is_child = true`).
 * 2. Close the unused read end of the pipe (`pipefd[0]`).
 * 3. Redirect `STDOUT_FILENO` to the pipe’s write end via `dup2()`.
 * 4. Close the write end after duplication to avoid file descriptor leaks.
 * 5. Recursively execute the left subtree of the AST.
 * 6. Exit the process using the current shell status.
 */
static void	execute_left_child(t_ast *node, t_shell *data, int pipefd[2])
{
	data->is_child = true;
	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(pipefd[1]);
		exit(EXIT_FAILURE);
	}
	close(pipefd[1]);
	execute_ast_tree(node->left, data);
	exit(data->status);
}

/**
 * @brief Fork and execute the left side of a pipeline.
 *
 * Creates a child process for the left command of a pipeline (e.g., `cmd1` in `cmd1 | cmd2`).
 * The child process will redirect its standard output to the pipe’s write end
 * and execute the left subtree of the AST via `execute_left_child()`.
 *
 * @param node AST node representing the pipeline.
 * @param data Shell state structure.
 * @param pipefd Array of two integers: pipefd[0] is read end, pipefd[1] is write end.
 * @return PID of the forked left child on success, -1 on failure.
 *
 * @details
 * The parent process receives the PID for later use in `waitpid()`.
 */
static	pid_t	fork_left_child(t_ast *node, t_shell *data, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork left");
		return (-1);
	}
	if (pid == 0)
		execute_left_child(node, data, pipefd);
	return (pid);
}

/**
 * @brief Execute the right side of a pipeline in a child process.
 *
 * Redirects standard input to the pipe’s read end so the child receives
 * the output from the left command. Closes unused file descriptors and
 * executes the right subtree of the AST.
 *
 * @param node AST node representing the pipeline.
 * @param data Shell state structure.
 * @param pipefd Array of two integers: pipefd[0] is read end, pipefd[1] is write end.
 *
 * @details
 * After execution, the child exits with the current shell status.
 * The read end is closed after duplication to prevent descriptor leaks.
 */
static void	execute_right_child(t_ast *node, t_shell *data, int pipefd[2])
{
	data->is_child = true;
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(pipefd[0]);
		exit(EXIT_FAILURE);
	}
	close(pipefd[0]);
	execute_ast_tree(node->right, data);
	exit(data->status);
}

/**
 * @brief Fork and execute the right side of a pipeline.
 *
 * This function creates a new child process to handle the right command
 * in a pipeline (e.g., `cmd2` in `cmd1 | cmd2`). The child redirects
 * its standard input to the pipe’s read end and executes the right
 * subtree of the AST.
 *
 * @param node Pointer to the current AST node representing the pipeline.
 * @param data Pointer to the shell state structure.
 * @param pipefd The pipe file descriptors; pipefd[0] is the read end,
 *               and pipefd[1] is the write end.
 *
 * @return The PID of the forked child process on success, or -1 on failure.
 *
 * @details
 * Steps performed:
 * 1. Call `fork()` to create a new process.
 * 2. In the child:
 *      - Execute the right side using `execute_right_child()`.
 * 3. In the parent:
 *      - Return the PID of the right child for later `waitpid()` calls.
 */
static pid_t	fork_right_child(t_ast *node, t_shell *data, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork right");
		return (-1);
	}
	if (pid == 0)
		execute_right_child(node, data, pipefd);
	return (pid);
}

/**
 * @brief Execute a pipeline node in the AST.
 *
 * Handles a pipeline between two commands (e.g., `cmd1 | cmd2`):
 * creates a pipe, forks two children, connects their input/output
 * through the pipe, and waits for both processes to finish.
 *
 * @param node AST node representing the pipeline.
 * @param data Shell state structure.
 * @return Exit status of the rightmost command in the pipeline,
 *         or EXIT_FAILURE on error.
 *
 * @details
 * On any failure (pipe creation or fork), ensures open file descriptors are
 * closed and previously forked children are waited for before returning.
 */
int	execute_pipeline(t_ast *node, t_shell *data)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (EXIT_FAILURE);
	}
	left_pid = fork_left_child(node, data, pipefd);
	if (left_pid == -1)
	{
		close_pipe_fds(pipefd);
		return (EXIT_FAILURE);
	}
	right_pid = fork_right_child(node, data, pipefd);
	if (right_pid == -1)
	{
		close_pipe_fds(pipefd);
		waitpid(left_pid, NULL, 0);
		return (EXIT_FAILURE);
	}
	close_pipe_fds(pipefd);
	//if (data->curr_ast)
	//	close_all_heredocs(data->curr_ast);
	return (wait_pipeline(left_pid, right_pid, data));
}
