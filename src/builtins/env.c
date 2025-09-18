#include "minishell.h"

/**
 * @brief Implements the builtin `env` command: prints environment variables.
 *
 * @param tokens Array of command tokens; should contain only "env".
 * @param data Pointer to the shell state structure (t_shell).
 * @return int 0 on successful execution, 1 if an error occurred.
 *
 * @details
 * - Prints all environment variables in data->env_list where
 *   env->in_env == true.
 * - If tokens contain extra arguments, prints an error and sets
 *   data->status = 2.
 * - If the environment list is missing (data->env_list == NULL) or
 *   a write error occurs during printing, sets data->status = 125.
 * - Return value 0 indicates successful execution; 1 indicates a
 *   non-fatal error that prevents the command from completing.
 */
int	builtin_env(char **tokens, t_shell *data)
{
	if (tokens && tokens[1])
	{
		print_error(ERR_ENV, ERR_TOO_MANY_ARGS, NULL, NULL);
		data->status = 2;
		return (EXIT_FAILURE);
	}
	if (!data->env_list)
	{
		data->status = 125;
		return (EXIT_FAILURE);
	}
	if (print_env_list(data->env_list) == -1)
	{
		data->status = 125;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
