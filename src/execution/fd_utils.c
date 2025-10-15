/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 11:58:59 by diade-so          #+#    #+#             */
/*   Updated: 2025/10/14 23:06:59 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(int *fd)
{
	if (fd && *fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}

// recursion to support nested shells
void	close_all_heredocs(t_ast *node)
{
	if (!node)
		return ;
	if (node->op_type == OP_HEREDOC && node->heredoc_fd >= 0)
		close_fds(&node->heredoc_fd);
	close_all_heredocs(node->left);
	close_all_heredocs(node->right);
	close_all_heredocs(node->next);
}

void	close_pipe_fds(int pipefd[2])
{
	close_fds(&pipefd[0]);
	close_fds(&pipefd[1]);
}

/**
 * @brief Save the standard file descriptors (stdin, stdout, stderr)
 *
 * Duplicates the three standard FDs so they can be restored later.
 * Initializes all elements to -1 to safely track partial failures.
 *
 * @param saved_fds Array of 3 ints to store the duplicated FDs.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE if any dup() fails.
 */
int	save_std_fds(int saved_fds[3])
{
	memset(saved_fds, -1, sizeof(int) * 3);
	saved_fds[0] = dup(STDIN_FILENO);
	if (saved_fds[0] == -1)
		return (perror("dup"), EXIT_FAILURE);
	saved_fds[1] = dup(STDOUT_FILENO);
	if (saved_fds[1] == -1)
	{
		perror("dup");
		close(saved_fds[0]);
		saved_fds[0] = -1;
		return (EXIT_FAILURE);
	}
	saved_fds[2] = dup(STDERR_FILENO);
	if (saved_fds[2] == -1)
	{
		perror("dup");
		close(saved_fds[0]);
		close(saved_fds[1]);
		saved_fds[0] = -1;
		saved_fds[1] = -1;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Restore the standard file descriptors (stdin, stdout, stderr) from saved copies.
 *
 * This function restores the three standard file descriptors using previously saved
 * duplicates in the `saved_fds` array. It safely handles cases where some FDs were
 * never saved (i.e., remain negative), preventing spurious errors.
 *
 * After restoring each FD, the corresponding entry in `saved_fds` is set to -1
 * to prevent accidental reuse.
 *
 * @param saved_fds An array of three integers containing saved duplicates of
 *                  STDIN_FILENO, STDOUT_FILENO, and STDERR_FILENO, in that order.
 *                  Values < 0 are ignored.
 *
 * @details
 * - Attempts to restore only valid (>= 0) saved FDs.
 * - If `dup2()` fails for any FD, prints an informative error via `perror()`,
 *   but continues restoring the remaining FDs.
 * - Closes each saved FD after restoration to avoid leaks.
 *
 * @note This function is useful for builtins or commands with temporary
 *       redirections where the original standard streams must be restored
 *       after execution.
 */
void	restore_std_fds(int saved_fds[3])
{
	//check first if saved FD is valid before attempting dup2 or close
	if (saved_fds[0] >= 0)
	{
		if (dup2(saved_fds[0], STDIN_FILENO) == -1)
			perror("restore stdin");
		close(saved_fds[0]);
		// after restoring set saved FD to -1 to prevent accidental double use
		saved_fds[0] = -1;
	}
	if (saved_fds[1] >= 0)
	{
		if (dup2(saved_fds[1], STDOUT_FILENO) == -1)
			perror("restore stdout");
		close(saved_fds[1]);
		saved_fds[1] = -1;
	}
	if (saved_fds[2] >= 0)
	{
		if (dup2(saved_fds[2], STDERR_FILENO) == -1)
			perror("restore stderr");
		close(saved_fds[2]);
		saved_fds[2] = -1;
	}
}
