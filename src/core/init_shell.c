#include "minishell.h"

/**
 * @brief Initialize the shell data structure.
 *
 * Responsibilities:
 * - Allocate and initialize the environment list from `envp`.
 * - Set initial status code to 0.
 * - Detect if input is from a terminal (`isatty`).
 * - Mark process as not a child (`is_child = false`).
 *
 * @param data  Pointer to the shell structure to initialize.
 * @param envp  Environment variables passed from `main`.
 *
 * @return 0 on success.
 * @return 1 on failure (e.g. environment initialization failed).
 */
int	init_shell(t_shell *data, char **envp)
{
	data->env_list = init_env_from_envp(envp);
	if (!data->env_list)
		return (1);
	data->status = 0;
	data->is_tty = isatty(STDIN_FILENO);
	data->is_child = false;
	// add more inits here if necessary
	return (0);
}
