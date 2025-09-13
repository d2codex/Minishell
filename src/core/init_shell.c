#include "minishell.h"

/**
 * @brief Increment the SHLVL environment variable.
 *
 * Updates the current shell level (SHLVL) by increasing its value
 * by 1 in the env list. If SHLVL does not exist, it is created
 * with an initial value of 1.
 *
 * @param env_list Environment list where SHLVL is stored.
 * @return 0 on success, 1 on error.
 */
static void	update_shlvl(t_list *env_list)
{
	t_env	*shlvl;
	int		level;
	int		out;
	//look for the key in the list
	shlvl = get_env_node_by_key(env_list, "SHLVL");
	// if found
	if (shlvl)
	{
		//get the value (level)
		level = ft_safe_atoi(shlvl->value, &out);
		//increase level by 1
		level++;
		free(shlvl->value);
		//set the value
		shlvl->value = ft_itoa(level);
	}
	else  //add the node to the list and set the value 
		set_env_node(&env_list, "SHLVL=1");
}

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
	update_shlvl(data->env_list);
	// add more inits here if necessary
	return (0);
}
