/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:40:26 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/15 17:40:27 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
static void	update_shlvl(t_list **env_list)
{
	t_env	*shlvl;
	int		level;
	int		status;
	char	*new_value;

	shlvl = get_env_node_by_key(*env_list, "SHLVL");
	if (shlvl)
	{
		status = ft_safe_atoi(shlvl->value, &level);
		if (status == 0 || level < 0)
			level = 0;
		level++;
		new_value = ft_itoa(level);
		if (!new_value)
		{
			ft_putendl_fd("failed to update SHLVL", 2);
			return ;
		}
		free(shlvl->value);
		shlvl->value = new_value;
	}
	else
	{
		set_env_node(env_list, "SHLVL=1");
	}
}

/**
 * @brief Initialize the shell data structure.
 *
 * Responsibilities:
 * - Allocate and initialize the environment list from `envp`.
 * - Set initial status code to 0.
 * - Detect if input is from a terminal (`isatty`).
 * - Mark process as not a child (`is_child = false`).
 * - Mark should_exit = false to start with infinite loop.
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
	update_shlvl(&data->env_list);
	data->status = 0;
	data->is_tty = isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
	data->is_child = false;
	data->should_exit = false;
	data->curr_ast = NULL;
	return (0);
}
