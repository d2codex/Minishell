#include "minishell.h"
#include <sys/ioctl.h>

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
 * Note: While rl_* functions are not in POSIX async-signal-safe list,
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
}

/**
 * @brief SIGINT handler for heredoc (ctrl-C)
 *
 * Injects a newline character into the input buffer using ioctl/TIOCSTI
 * to force readline to return immediately, allowing clean heredoc interruption.
 *
 * @param sig Signal number (SIGINT = 2)
 */
void	handle_sigint_heredoc(int sig)
{
	char	newline;

	g_signal_received = sig;
	newline = '\n';
	ioctl(STDIN_FILENO, TIOCSTI, &newline);
}

/**
 * @brief SIGQUIT handler (ctrl-\)
 *
 * Does nothing in interactive mode at prompt. Just void out the param.
 * We need handler to have the same signature in order to pass them to the
 * sigaction structure (just like what we did with builtins)
 * For, child processes, SIG_DFL flag will be used for proper termination.
 *
 * @param sig Signal number (SIGQUIT = 3)
 */
void	handle_sigquit(int sig)
{
	(void)sig;
}
