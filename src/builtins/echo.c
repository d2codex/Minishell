#include "minishell.h"

/**
 * @brief Check if token is a valid bash style -n option for echo.
 *
 * Accepts "-n", "-nn", "-nnn", etc. but rejects "-na", "-n1", etc.
 * Implements bash's quirky but exact -n recognition behavior.
 *
 * @param token String to check
 * @return true if valid -n option, false otherwise
 */
static bool	is_echo_n_option(const char *token)
{
	int	i;

	if (!token || token[0] != '-' || token[1] != 'n')
		return (false);
	i = 2;
	while (token[i])
	{
		if (token[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

/**
 * @brief Print tokens starting at given index, space separated.
 *
 * Prints each token with single spaces between them (no space after last).
 * Helper function for echo output formatting.
 *
 * @param tokens NULL-terminated array of strings
 * @param start_index Index to start printing from
 */
static void	print_echo_cmd(char **tokens, int start_print_index)
{
	while (tokens[start_print_index] != NULL)
	{
		printf("%s", tokens[start_print_index]);
		if (tokens[start_print_index + 1])
			printf(" ");
		start_print_index++;
	}
}

/**
 * @brief Implements the echo builtin command.
 *
 * Prints the given arguments to standard output, separated by spaces.
 * Supports the -n option to suppress the trailing newline. Multiple
 * consecutive -n options are all consumed and treated as a single option.
 *
 * Behavior:
 * - echo (no args): prints a newline
 * - echo args: prints args separated by spaces, followed by newline
 * - echo -n: prints nothing (no newline)
 * - echo -n args: prints args separated by spaces, no trailing newline
 * - echo -n -n -n args: same as echo -n args (multiple -n consumed)
 * - echo -n -nnnnnnnnnnnn: chained options, works like -n
 *
 * The -n option is only effective when it appears consecutively at the
 * beginning of the argument list.
 *
 * @param tokens Command tokens array where tokens[0] is "echo"
 * @param data Shell data structure (unused in this builtin)
 * @return Always returns EXIT_SUCCESS (0), updates `data->status`.
 */
int	builtin_echo(char **tokens, t_shell *data)
{
	int		start_print_index;
	bool	print_new_line;

	if (tokens[1] == NULL)
	{
		printf("\n");
		data->status = EXIT_SUCCESS;
		return (data->status);
	}
	start_print_index = 1;
	print_new_line = true;
	while (tokens[start_print_index]
		&& is_echo_n_option(tokens[start_print_index]))
	{
		print_new_line = false;
		start_print_index++;
	}
	print_echo_cmd(tokens, start_print_index);
	if (print_new_line)
		printf("\n");
	data->status = EXIT_SUCCESS;
	return (data->status);
}
