#include "minishell.h"

/**
 * @brief Print arguments from tokens array starting at given index.
 *
 * Iterates through the tokens array from the specified starting index,
 * printing each token followed by a space (except for the last token).
 * This helper function handles the actual output formatting for echo.
 *
 * @param tokens Array of string tokens to print
 * @param i Starting index in the tokens array
 */
static void	print_echo_cmd(char **tokens, int i)
{
	while (tokens[i] != NULL)
	{
		printf("%s", tokens[i]);
		// only print space if you are NOT at the end of the argument list
		if (tokens[i + 1])
			printf(" ");
		i++;
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
 *
 * The -n option is only effective when it appears consecutively at the
 * beginning of the argument list.
 *
 * @param tokens Command tokens array where tokens[0] is "echo"
 * @param data Shell data structure (unused in this builtin)
 * @return Always returns 0
 */
int	builtin_echo(char **tokens, t_shell *data)
{
	int		start_print_index;
	bool	print_new_line;

	(void)data;
	// early return for command without argument so that we don't fall into
	// the while loop which would try to access tokens[1] with a NULL pointer
	// this prevents SEGFAULT
	if (tokens[1] == NULL)
	{
		printf("\n");
		return (0);
	}
	// scan for consecutive -n options at the beginning
	start_print_index = 1;
	print_new_line = true;
	while (tokens[start_print_index]
		&& ft_strcmp(tokens[start_print_index], "-n") == 0)
	{
		print_new_line = false;  // at least one -n found, means we want no \n
		start_print_index++;
	}
	// printing helper because of the norm, right index for any cases
	print_echo_cmd(tokens, start_print_index);
	if (print_new_line)
		printf("\n");
	return (0);
}
