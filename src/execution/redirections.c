#include "minishell.h"

/**
 * @brief Determine the target file descriptor for a redirection.
 *
 * This function inspects the given AST node's operator type to decide whether
 * the redirection targets the standard input or output stream.
 *
 * @param node Pointer to the AST node representing the redirection.
 * @return STDIN_FILENO if the redirection is an input or heredoc,
 *         otherwise STDOUT_FILENO.
 */
static int	get_target_fd(t_ast *node)
{
	if (node->op_type == OP_INPUT || node->op_type == OP_HEREDOC)
		return (STDIN_FILENO);
	return (STDOUT_FILENO);
}

/**
 * @brief Perform a file descriptor duplication for redirection.
 *
 * This function duplicates the provided file descriptor onto the specified
 * target descriptor (usually STDIN or STDOUT). It also handles cleanup and
 * error propagation. If duplication fails and the process is a child, it exits
 * immediately with failure.
 *
 * @param fd The source file descriptor to duplicate.
 * @param target_fd The target file descriptor (STDIN or STDOUT).
 * @param node Pointer to the AST node (used for context in error handling).
 * @param data Pointer to the main shell data structure.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
static int	perform_dup(int fd, int target_fd, t_shell *data)
{
	if (dup2(fd, target_fd) == -1) //stdin now reads from fd
	{
		perror("dup2");
		close(fd);
		data->status = EXIT_FAILURE;
		if (data->is_child)
			exit(data->status);
		return (data->status);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Redirect a command's standard stream based on its AST node.
 *
 * This function determines whether the redirection should affect STDIN or
 * STDOUT, and then performs the appropriate duplication. It delegates the
 * low-level duplication logic to `perform_dup()`.
 *
 * @param node Pointer to the AST node representing the redirection.
 * @param data Pointer to the shell data structure for error handling.
 * @param fd The file descriptor to redirect from/to.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
static int	dup_redirection(t_ast *node, t_shell *data, int fd)
{
	int	target_fd;
	int	result;

	target_fd = get_target_fd(node);
	result = perform_dup(fd, target_fd, data);
	return (result);
}

/**
 * @brief Apply a single redirection from an AST node.
 *
 * This function handles the setup of one redirection operation. It opens the
 * target file (or uses the heredoc file descriptor) depending on the operator
 * type, and then duplicates the resulting file descriptor to either standard
 * input or output using `dup_redirection()`.
 *
 * It gracefully handles file open errors, updating the shell status and exiting
 * immediately if the process is a child.
 *
 * @param node Pointer to the AST node representing the redirection.
 * @param data Pointer to the main shell data structure.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
static int	apply_single_redirection(t_ast *node, t_shell *data)
{
	int	fd;
	int	result;

	fd = -1;
	if (node->op_type == OP_INPUT)
		fd = open(node->filename, O_RDONLY);
	else if (node->op_type == OP_OUTPUT)
		fd = open(node->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (node->op_type == OP_APPEND)
		fd = open(node->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (node->op_type == OP_HEREDOC)
		fd = node->heredoc_fd;
	if (fd < 0)
	{
		perror(node->filename);
		data->status = EXIT_FAILURE;
		if (data->is_child)
			exit(data->status);
		return (data->status);
	}
	result = dup_redirection(node, data, fd);
	// mark heredoc fd as closed
	if (node->op_type == OP_HEREDOC && result == EXIT_SUCCESS)
		node->heredoc_fd = -1;
	// always close origianl FD after successful dup2
//	if (result == EXIT_SUCCESS)
	close_fds(&fd);
	return (result);
}

/**
 * @brief Apply all redirections linked to a command node.
 *
 * Traverses the chain of redirection nodes (linked through the `right` pointer)
 * and applies each one in sequence using `apply_single_redirection()`.
 *
 * If any redirection fails, the function immediately stops and returns
 * `EXIT_FAILURE`, leaving subsequent redirections unapplied.
 *
 * @param node Pointer to the first redirection node (NODE_REDIR) in the AST.
 * @param data Pointer to the main shell data structure.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE if any redirection fails.
 */
int	apply_redirections(t_ast *node, t_shell *data)
{
	int	ret;

	while (node && node->type == NODE_REDIR)
	{
		ret = apply_single_redirection(node, data);
		if (ret != EXIT_SUCCESS)
			return (ret);
		node = node->right;
	}
	return (EXIT_SUCCESS);
}
