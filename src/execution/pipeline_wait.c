/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_wait.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 18:01:16 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/15 18:08:53 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Extract and update the shell status from a child process.
 *
 * This function interprets the exit information returned by `waitpid()`
 * for a child process in a pipeline. If the process exited normally,
 * it updates `data->status` with its exit code. Otherwise, it sets the
 * shell status to `EXIT_FAILURE`.
 *
 * @param status The raw status value returned by `waitpid()`.
 * @param data   Pointer to the shell state structure.
 *
 * @return The updated shell status.
 */
int	handle_pipeline_status(int status, t_shell *data)
{
	int	sig;

	if (WIFEXITED(status))
		data->status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		data->status = 128 + sig;
	}
	else
		data->status = EXIT_FAILURE;
	return (data->status);
}

/**
 * @brief Wait for both child processes in a pipeline to complete.
 *
 * This function closes the pipe file descriptors in the parent process
 * and waits for both the left and right child processes created by
 * `execute_pipeline()` to terminate. The final exit status of the
 * pipeline is determined by the rightmost command.
 *
 * @param left_pid  PID of the left child process.
 * @param right_pid PID of the right child process.
 * @param pipefd    The pipe file descriptors; both ends are closed here.
 * @param data      Pointer to the shell state structure.
 *
 * @return The exit status of the rightmost command in the pipeline.
 *
 * @details
 * Steps performed:
 * 1. Close both ends of the pipe in the parent process.
 * 2. Wait for the left and right child processes using `waitpid()`.
 * 3. Extract and store the right child’s exit status via
 *    `handle_pipeline_status()`.
 *
 * The left child’s exit status is ignored, as shell semantics define
 * the pipeline’s return status to be that of the last command.
 */
int	wait_pipeline(pid_t left_pid, pid_t right_pid, t_shell *data)
{
	int	status_left;
	int	status_right;
	int	sig;

	setup_signals_ignore();
	waitpid(left_pid, &status_left, 0);
	waitpid(right_pid, &status_right, 0);
	setup_signals_interactive();
	if (WIFSIGNALED(status_right))
	{
		sig = WTERMSIG(status_right);
		if (sig == SIGQUIT)
			write(1, "Quit (core dumped)\n", 20);
		else if (sig == SIGINT)
			write(1, "\n", 1);
	}
	if (data->curr_ast)
		close_all_heredocs(data->curr_ast);
	return (handle_pipeline_status(status_right, data));
}
