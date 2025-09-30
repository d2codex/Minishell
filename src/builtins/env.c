#include "minishell.h"

/**
 * @brief Builtin command: print the environment variables.
 *
 * Prints all environment variables in `data->env_list`. Handles errors such as
 * extra arguments (MISUSAGE_ERROR) or missing/unprintable environment
 * (INTERNAL_ERROR).
 * Updates `data->status` with the command's exit code.
 *
 * @param tokens Command tokens from user input.
 *               - tokens[0] should be "env"
 *               - tokens[1+] triggers "too many arguments" error
 * @param data Shell state, including environment and exit status.
 * @return Exit status of the command (0 on success, >0 on failure).
 */
int	builtin_env(char **tokens, t_shell *data)
{
	if (tokens && tokens[1])
	{
		print_error(ERR_PREFIX, ERR_ENV, ERR_TOO_MANY_ARGS, NULL);
		data->status = MISUSAGE_ERROR;
		return (data->status);
	}
	if (!data->env_list)
	{
		data->status = INTERNAL_ERROR;
		return (data->status);
	}
	if (print_env_list(data->env_list) == -1)
	{
		data->status = INTERNAL_ERROR;
		return (data->status);
	}
	data->status = EXIT_SUCCESS;
	return (data->status);
}
