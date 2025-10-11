#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

/**
 * @brief SIGINT handler (ctrl-C)
 *
 * Uses readline functions to cleanly handle Ctrl+C:
 * - Writes newline to move cursor
 * - Tells readline we're on a new line
 * - Clears the input buffer
 * - Redisplays the prompt
 *
 * Note: While rl_* functions aren't in POSIX async-signal-safe list,
 * they are specifically designed by readline for use in signal handlers
 * and are authorized in the subject.
 *
 * @param sig Signal number (SIGINT = 2)
 */
void	handle_sigint(int sig)
{
	g_signal_received = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	// OLD APPROACH (caused "Bad file descriptor" errors):
	// close(STDIN_FILENO);
}

/**
 * @brief SIGQUIT handler (ctrl-\)
 *
 * Does nothing in interactive mode at prompt.
 * Child processes will use SIG_DFL for proper termination.
 *
 * @param sig Signal number (SIGQUIT = 3)
 */
void	handle_sigquit(int sig)
{
	(void)sig;
}
