#include "minishell.h"

/**
 * @brief Handle `exit` when too many arguments are provided.
 *
 * Prints an error message and sets `data->status` to EXIT_FAILURE (1). 
 * The shell does not exit in this case.
 *
 * @param data Shell state, including exit status.
 * @return EXIT_FAILURE (1) as the exit status.
 */
static int	handle_exit_too_many_args(t_shell *data)
{
	print_error(ERR_PREFIX, ERR_EXIT, ERR_TOO_MANY_ARGS, NULL);
	data->status = EXIT_FAILURE; // general error (1)
	return (data->status);
}

/**
 * @brief Handle `exit` with an invalid numeric argument.
 *
 * Prints an error message, sets `data->status` to MISUSAGE_ERROR (2),
 * and signals the shell to exit via `data->should_exit`. If running in 
 * a child process, exits immediately.
 *
 * @param arg The invalid argument passed to `exit`.
 * @param data Shell state, including exit status, TTY flags, and exit flag.
 * @return MISUSAGE_ERROR (2) as the exit status.
 */
static int	handle_exit_invalid_arg(char *arg, t_shell *data)
{
	print_error(ERR_PREFIX, ERR_EXIT, arg, ERR_NUMERIC_ARG);
	data->status = MISUSAGE_ERROR;
	if (data->is_child)
		exit(data->status);
	data->should_exit = true;
	return (data->status);
}

/**
 * @brief Builtin command: exit the shell.
 *
 * Signals the shell to terminate by setting `data->should_exit`. 
 * If an exit code is provided, updates `data->status` accordingly. 
 * Handles invalid numeric arguments and too many arguments. 
 * If running in a child process, exits immediately.
 *
 * @param tokens Command tokens from user input.
 *               - tokens[0] should be "exit"
 *               - tokens[1] (optional) numeric exit code
 *               - tokens[2+] triggers "too many arguments" error
 * @param data Shell state, including exit status, TTY flags, and exit flag.
 * @return Exit status to store in `data->status`.
 */
int	builtin_exit(char **tokens, t_shell *data)
{
	long long	exit_code;

	if (data->is_tty && !data->is_child)
		printf("exit\n");
	if (tokens[1] == NULL)
	{
		data->should_exit = true; // signal main loop to stop
		if (data->is_child)
			exit(data->status); // terminate child immediately
		return (data->status); // 0 at this point
	}
	if (ft_safe_atoll(tokens[1], &exit_code) != 1)
		return (handle_exit_invalid_arg(tokens[1], data));
	if (tokens[2] != NULL)
		return (handle_exit_too_many_args(data));
	data->status = (unsigned char)(exit_code);
	if (data->is_child)
		exit(data->status);
	data->should_exit = true;
	return (data->status);
}
