#include "minishell.h"

/**
 * @brief Configures signals for interactive mode (parent shell)
 *
 * - SIGINT (ctrl-C): custom handler to display new prompt
 * - SIGQUIT (ctrl-\): ignored at prompt
 */
void	setup_signals_interactive(void)
{
	struct sigaction	sa_int; // config for SIGINT (struct comes from <signal.h>)
	struct sigaction	sa_quit; // config for SIGQUIT

	// Configure SIGINT (ctrl-C)
	sigemptyset(&sa_int.sa_mask); // empty signal mask (don't block other signals)
	sa_int.sa_flags = 0; // no special flags needed for our case
	sa_int.sa_handler = handle_sigint;  // set our custom handler function
	sigaction(SIGINT, &sa_int, NULL); // install the config for SIGINT

	// Configure SIGQUIT (ctrl-\)
	sigemptyset(&sa_quit.sa_mask); // empty signal mask
	sa_quit.sa_flags = 0; // no special flags
	sa_quit.sa_handler = SIG_IGN; // ignore this signal (SIG_IGN specail values that comes from <signal.h>)
	sigaction(SIGQUIT, &sa_quit, NULL); // install the config for SIGQUIT
}

/**
 * @brief Restores default signal handlers for child processes
 *
 * External commands must react normally to signals (terminate on ctrl-C, etc.)
 * We use SIG_DFL flag from <signal.h> to restore kernel's default behavior.
 */
void	setup_signals_child(void)
{
	struct sigaction	signal_action;  // Single config for both signals (same settings)

	// Configure both SIGINT and SIGQUIT with default behavior
	sigemptyset(&signal_action.sa_mask); // Empty signal mask (don't block other signals)
	signal_action.sa_flags = 0; // No special flags
	signal_action.sa_handler = SIG_DFL; // Restore default behavior (kernel handles it)

	sigaction(SIGINT, &signal_action, NULL); // Install default for SIGINT (ctrl-C)
	sigaction(SIGQUIT, &signal_action, NULL); // Install default for SIGQUIT (ctrl-\)
}

/**
 * @brief Ignores signals while parent waits for child command
 *
 * When executing external commands, the parent shell should NOT react to
 * ctrl-C or ctrl-\ - only the child process should handle them.
 * This prevents double prompts and ensures clean signal handling.
 */
void	setup_signals_ignore(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
