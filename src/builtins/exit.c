#include "minishell.h"

/**
 * @brief Handle exit without arguments case.
 *
 * Uses the current status stored in data->status as the exit code.
 * - Parent shell: returns -1 to signal minishell_loop() to terminate cleanly.
 * - Child process: calls exit() directly with data->status.
 *
 * @param data Shell data structure
 * @return -1 to signal shell termination (parent), does not return for child
 */

static int	handle_exit_no_args(t_shell *data)
{
	// child process: terminate immediately with current status
	if (data->is_child)
		exit(data->status);
	// parent process: signal main loop to exit cleanly
	return (-1);
}

/**
 * @brief Handle exit with too many arguments case.
 *
 * Prints an error message and sets status = 1 (general error).
 * This is the only exit case where the shell does NOT terminate:
 * - Parent shell: continues running after showing the error.
 * - Child process: normal behavior, continues as well.
 *
 * @param data Shell data structure
 * @return 0 to continue shell execution
 */

static int	handle_exit_too_many_args(t_shell *data)
{
	print_error_multi(ERR_PREFIX, ERR_EXIT, ERR_TOO_MANY_ARGS, NULL);
	// set general error status
	data->status = 1;
	// special case: shell continues running (only exit case that doesn't quit)
	return (0);
}

/**
 * @brief Handle exit with invalid numeric argument case.
 *
 * Prints error message and sets status = 2 (syntax/usage error).
 * Behavior differs by process context:
 * - Parent process: returns -1 to signal clean termination via minishell_loop()
 * - Child process: calls exit(2) directly to terminate immediately
 *
 * This matches bash behavior where invalid arguments always terminate the shell
 *
 * @param arg The invalid argument string
 * @param data Shell data structure
 * @return -1 to signal shell termination (parent), does not return for child
 */
static int	handle_exit_invalid_arg(char *arg, t_shell *data)
{
	print_error_multi(ERR_PREFIX, ERR_EXIT, arg, ERR_NUMERIC_ARG);
	data->status = 2;
	// child process: terminate immediately with error code
	if (data->is_child)
		exit(2);
	// parent process: signal main loop to exit cleanly
	return (-1);
}

/**
 * @brief Implementation of the exit builtin command for minishell.
 *
 * Handles shell termination with proper exit code management following bash
 * behavior. Argument validation order matches bash: validity checked before
 * argument count.
 *
 * Process handling:
 * - Parent process: returns -1 to signal minishell_loop() for clean exit
 * - Child process: calls exit() directly with computed exit code
 *
 * @param tokens Command tokens array where tokens[0] is "exit"
 * @param data Shell data structure containing status and environment
 * @return -1 to signal shell exit, 0 to continue shell (too many args only)
 *
 * @note Exit codes use (unsigned char) cast for automatic modulo 256 conversion
 * This handles negative numbers and values >255 correctly like bash
 * @note Only "too many arguments" case continues shell execution
 */
int builtin_exit(char **tokens, t_shell *data)
{
	long long exit_code;

	// print "exit" only in interactive mode
	if (data->is_tty && !data->is_child)
		printf("exit\n");

	// no argument - use current status
	if (tokens[1] == NULL)
		return (handle_exit_no_args(data));

	// check argument validity FIRST (bash order)
	// invalid argument takes precedence over "too many arguments"
	if (ft_safe_atoll(tokens[1], &exit_code) != 1)
		return (handle_exit_invalid_arg(tokens[1], data));

	// valid first argument but too many total arguments
	if (tokens[2] != NULL)
		return (handle_exit_too_many_args(data));

	// case 4: Single valid numeric argument
	// cast to unsigned char for automatic modulo 256 (handles negatives correctly)
	data->status = (unsigned char)(exit_code);
	// child process: terminate immediately with the computed code
	if (data->is_child)
		exit((unsigned char)(exit_code));
	// parent process: signal main loop to exit cleanly
	return (-1);
}
