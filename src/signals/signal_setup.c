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
	struct sigaction sa_interactive;

	memset(&sa_interactive, 0, sizeof(sa_interactive));   // zero all fields
	sigemptyset(&sa_interactive.sa_mask);     // no signals blocked during handler
	sa_interactive.sa_handler = handle_sigint;
	sigaction(SIGINT, &sa_interactive, NULL); // install SIGINT handler

	sa_interactive.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_interactive, NULL); // ignore SIGQUIT
}

/**
 * @brief Restore default signal handlers for child processes.
 *
 * External commands must react normally (terminate on Ctrl-C, dump core on Ctrl-\).
 * SIG_DFL restores the kernel's default behavior.
 */
void	setup_signals_child(void)
{
	struct sigaction sa_child;

	memset(&sa_child, 0, sizeof(sa_child));
	sigemptyset(&sa_child.sa_mask);
	sa_child.sa_handler = SIG_DFL;

	sigaction(SIGINT, &sa_child, NULL);
	sigaction(SIGQUIT, &sa_child, NULL);
}

/**
 * @brief Ignore signals while parent waits for a child process.
 *
 * When running external commands, only the child should react to signals.
 * This prevents duplicate prompts and unwanted interruptions in the parent.
 */
void	setup_signals_ignore(void)
{
	struct sigaction sa_ignore;

	memset(&sa_ignore, 0, sizeof(sa_ignore));
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_handler = SIG_IGN;

	sigaction(SIGINT, &sa_ignore, NULL);
	sigaction(SIGQUIT, &sa_ignore, NULL);
}

/**
 * @brief Configure signals for heredoc input.
 *
 * - SIGINT (Ctrl-C): custom handler without rl_redisplay()
 * - SIGQUIT (Ctrl-\): ignored
 *
 * This prevents the heredoc prompt from being redisplayed after Ctrl-C,
 * allowing clean interruption of heredoc input.
 */
void	setup_signals_heredoc(void)
{
	struct sigaction sa_heredoc;

	memset(&sa_heredoc, 0, sizeof(sa_heredoc));
	sigemptyset(&sa_heredoc.sa_mask);
	sa_heredoc.sa_handler = handle_sigint_heredoc;
	sigaction(SIGINT, &sa_heredoc, NULL);

	sa_heredoc.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_heredoc, NULL);
}
