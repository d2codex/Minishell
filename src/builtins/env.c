#include "minishell.h"

/**
 * @brief Implements the `env` builtin command.
 *
 * Prints all environment variables marked as in_env to stdout.
 * Ignores any arguments (no options or additional commands supported).
 *
 * @param tokens Unused; placeholder for command arguments.
 * @param data Pointer to the shell state, containing the environment list.
 * @return int Exit status (always 0 for success).
 */
int	builtin_env(char **tokens, t_shell *data)
{
	(void)tokens;
	print_env_list(data->env_list);
	return (0);
}
