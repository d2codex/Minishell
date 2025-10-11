#include "minishell.h"

/*
 * OLD APPROACH: save/restore stdin method
 * These functions are no longer needed with the readline-native signal handling.
 * Kept here commented for reference in case we need to revert.
 */

// /**
//  * @brief Duplicates stdin for later restoration
//  *
//  * @return Duplicated fd on success, -1 on error
//  */
// int	save_stdin(void)
// {
// 	int	saved_stdin;
//
// 	saved_stdin = dup(STDIN_FILENO);
// 	if (saved_stdin == -1)
// 		perror("dup");
// 	return (saved_stdin);
// }

// /**
//  * @brief Restores stdin from saved fd and closes the backup
//  *
//  * @param saved_stdin Previously saved stdin fd
//  * @return EXIT_SUCCESS on success, EXIT_FAILURE on error
//  */
// int	restore_stdin(int saved_stdin)
// {
// 	if (saved_stdin == -1)
// 		return (EXIT_FAILURE);
// 	if (dup2(saved_stdin, STDIN_FILENO) == -1)
// 	{
// 		perror("dup2");
// 		close(saved_stdin);
// 		return (EXIT_FAILURE);
// 	}
// 	if (close(saved_stdin) == -1)
// 	{
// 		perror("close");
// 		return (EXIT_FAILURE);
// 	}
// 	return (EXIT_SUCCESS);
// }

// /**
//  * @brief Checks if readline() returned NULL due to signal vs EOF
//  *
//  * @return true if was a signal (continue loop), false if real EOF (exit)
//  */
// bool	check_signal_after_readline(char **line, t_shell *data, int saved_stdin)
// {
// 	if (*line == NULL && g_signal_received == SIGINT)
// 	{
// 		restore_stdin(saved_stdin);
// 		g_signal_received = 0;
// 		data->status = EXIT_SIGINT;
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		*line = ft_strdup("");
// 		return (true); // was signal, continue loop
// 	}
// 	return (false); // was real EOF, should exit
// }
