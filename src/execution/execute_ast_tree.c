#include "minishell.h"

/**
 * @brief Execute a single command (builtin or external) without forking.
 *
 * If the AST node represents a builtin, it executes it via `execute_builtin`.
 * Otherwise, it executes an external command using `execute_external_command`.
 *
 * @param node AST node representing the command.
 * @param data Pointer to the shell state structure.
 * @return int The exit status of the command. For builtins, it is `data->status`;
 *             for external commands, `data->status` is updated by `execute_external_command`.
 */
static int	execute_command(t_ast *node, t_shell *data)
{
	if (!node || node->type != NODE_CMD)
		return (EXIT_SUCCESS);
	if (is_builtin(node))
		return (execute_builtin(node, data));
	else
		execute_external_command(node->argv, data);
	return (data->status);
}

/**
 * @brief Execute a command in a child process, applying redirections if needed.
 *
 * Forks a new child process, applies any redirections on the right side of the node,
 * then executes the command (builtin or external) inside the child. The parent waits
 * for the child to finish and updates `data->status` with the child’s exit status.
 *
 * @param node AST node representing the command to execute in the child.
 * @param data Pointer to the shell state structure.
 * @return int The exit status of the child process, stored in `data->status`.
 */
static int	execute_in_child_process(t_ast *node, t_shell *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (EXIT_FAILURE);
	}
	if (pid == 0) // child
	{
		data->is_child = true;
		if (apply_redirections(node->right, data) != EXIT_SUCCESS)
		{
			close_all_heredocs(node->right);
			exit (data->status);
		}
		exit(execute_command(node, data));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->status = WEXITSTATUS(status);
	else
		data->status = EXIT_FAILURE;
	return (data->status);
}

/**
 * @brief Recursively execute an AST tree representing commands and pipelines.
 *
 * Handles pipelines, builtins (forking and non-forking), and external commands.
 * Redirections are applied before execution. The function decides when to fork
 * for commands that should run in a separate process.
 *
 * @param node Root AST node of the tree/subtree to execute.
 * @param data Pointer to the shell state structure.
 * @return int The exit status of the last executed command in the tree,
 *             stored in `data->status`.
 */
int	execute_ast_tree(t_ast *node, t_shell *data)
{
	int	saved_fds[3];

	if (!node)
		return (EXIT_SUCCESS);
	// handle pipelines - always forks
	if (node->type == NODE_PIPE)
		return (execute_pipeline(node, data));
	// Non-forking builtins: cd, export, unset, exit
	if (node->type == NODE_CMD && is_nonforking_builtin(node))
	{
		// case 1: builtin without redirections - run directly
		if (!node->right)
		{
			data->status = execute_builtin(node, data); //runs in parent
			return (data->status);
		}
		// case 2: builtin WITH redirections - temporarily redirect parent
		if(save_std_fds(saved_fds) == -1)
			return (EXIT_FAILURE);
		if (apply_redirections(node->right, data) == EXIT_SUCCESS)
			data->status = execute_builtin(node, data);
		restore_std_fds(saved_fds);
		close_all_heredocs(node->right);
		return (data->status);
	}
	// forking commands (external or forkable builtins)
	if (should_fork(node, data))
		return (execute_in_child_process(node, data));
	// simiple builtin or cmd - execute directly
	data->status = execute_command(node, data);
	return (data->status);
}
