/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:41:15 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/15 19:29:25 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles SIGINT interruption during heredoc input.
 *
 * @param line Current line being read (may be NULL)
 * @param pipefd Pipe file descriptors to close
 * @return Always returns -1 to indicate interruption
 */
static int	handle_heredoc_interrupt(char *line, int pipefd[2])
{
	if (line)
		free(line);
	close(pipefd[0]);
	close(pipefd[1]);
	setup_signals_interactive();
	return (-1);
}

/**
 * @brief Handles error case when heredoc creation fails.
 *
 * @param node AST node where the error occurred
 * @param data Shell state structure
 */
static void	handle_heredoc_error(t_ast *node, t_shell *data)
{
	close_all_heredocs(node);
	if (g_signal_received == SIGINT)
	{
		data->status = EXIT_SIGINT;
		g_signal_received = 0;
	}
	else
		data->status = EXIT_FAILURE;
}

/**
 * @brief Reads heredoc lines until limiter or interruption.
 *
 * @param limiter Delimiter string
 * @param pipefd Pipe file descriptors
 * @return 0 on success, -1 on interruption
 */
static int	read_heredoc_lines(const char *limiter, int pipefd[2])
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (g_signal_received == SIGINT)
			return (handle_heredoc_interrupt(line, pipefd));
		if (!line)
		{
			print_error(ERR_PREFIX, ERR_HEREDOC_EOF, NULL, NULL);
			break ;
		}
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, pipefd[1]);
		free(line);
	}
	return (0);
}

/**
 * @brief Creates a pipe and fills it with the contents of a heredoc.
 *
 * This function reads lines from the terminal until the specified
 * limiter is reached. Each line is written to the write end of a
 * newly created pipe. The write end is closed after all lines
 * are written, leaving the read end open for later use in input
 * redirection.
 *
 * @param limiter The string that ends the heredoc input.
 * @return The file descriptor of the read end of the pipe on success,
 *         or -1 if pipe creation fails.
 *
 * @details
 * - Uses `readline()` to read input from the user.
 * - Writes each line followed by a newline to the pipe.
 * - Frees each line after writing.
 * - Closes the write end of the pipe to signal EOF to readers.
 */
static int	create_heredoc_fd(const char *limiter)
{
	int	pipefd[2];
	int	status;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	setup_signals_heredoc();
	status = read_heredoc_lines(limiter, pipefd);
	close(pipefd[1]);
	setup_signals_interactive();
	if (status == -1)
		return (-1);
	return (pipefd[0]);
}

/**
 * @brief Preprocesses all heredoc redirections in an AST.
 *
 * Recursively traverses the abstract syntax tree (AST) and, for
 * each heredoc redirection node, creates a pipe containing the
 * heredoc input. The read file descriptor is stored in the node’s
 * `heredoc_fd` member for later use during execution.
 *
 * @param node Pointer to the current AST node.
 * @param data Pointer to the shell state structure.
 * @return EXIT_SUCCESS if all heredocs are processed successfully,
 *         EXIT_FAILURE if any heredoc fails.
 *
 * @details
 * - Traverses the left subtree first, then the current node, then
 *   the right subtree (in-order traversal).
 * - For each `NODE_REDIR` node with `op_type == OP_HEREDOC`,
 *   calls `create_heredoc_fd()` to generate the pipe.
 * - On failure, sets `data->status` to `EXIT_FAILURE` and
 *   propagates the error.
 */
int	preprocess_heredocs(t_ast *node, t_shell *data)
{
	int	fd;

	if (!node)
		return (EXIT_SUCCESS);
	if (node->left)
		if (preprocess_heredocs(node->left, data) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
	if (node->type == NODE_REDIR && node->op_type == OP_HEREDOC)
	{
		fd = create_heredoc_fd(node->filename);
		if (fd == -1)
			return (handle_heredoc_error(node, data), EXIT_FAILURE);
		node->heredoc_fd = fd;
	}
	if (node->right)
		if (preprocess_heredocs(node->right, data) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
	if (node->next)
		if (preprocess_heredocs(node->next, data) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
