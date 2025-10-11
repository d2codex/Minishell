#include "minishell.h"

/**
 * @brief Configure signals for interactive shell (parent).
 *
 * - SIGINT (Ctrl-C): custom handler to display a new prompt
 * - SIGQUIT (Ctrl-\): ignored
 *
 * Using memset to initialize the struct to 0 simplifies setup.
 */
void	setup_signals_interactive(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));   // zero all fields
	sigemptyset(&sa.sa_mask);     // no signals blocked during handler
	sa.sa_handler = handle_sigint;
	sigaction(SIGINT, &sa, NULL); // install SIGINT handler

	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL); // ignore SIGQUIT
}

/**
 * @brief Restore default signal handlers for child processes.
 *
 * External commands must react normally (terminate on Ctrl-C, dump core on Ctrl-\).
 * SIG_DFL restores the kernel's default behavior.
 */
void	setup_signals_child(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_DFL;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/**
 * @brief Ignore signals while parent waits for a child process.
 *
 * When running external commands, only the child should react to signals.
 * This prevents duplicate prompts and unwanted interruptions in the parent.
 */
void	setup_signals_ignore(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
