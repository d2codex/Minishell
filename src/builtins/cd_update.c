/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_update.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 17:56:35 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/16 17:56:49 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Helper to update or create an environment variable.
 *
 * Builds "KEY=VALUE" and reuses set_env_node() to update or create it.
 * On allocation failure, does nothing.
 *
 * @param env_list Pointer to the environment list.
 * @param key      The variable name (e.g., "PWD", "OLDPWD").
 * @param value    The new value for the variable.
 */
void	update_env_var_value(t_list **env_list, const char *key,
	const char *value)
{
	char	*joined;
	char	*full;

	if (!env_list || !key || !value)
		return ;
	joined = ft_strjoin("=", value);
	if (!joined)
		return ;
	full = ft_strjoin(key, joined);
	free(joined);
	if (!full)
		return ;
	set_env_node(env_list, full);
	free(full);
}

/**
 * @brief Update PWD after a successful cd.
 *
 * If getcwd() fails, prints an error but does not abort.
 *
 * @param data Shell data structure.
 */
void	update_pwd_from_target(t_shell *data)
{
	char	*cwd;

	if (!data)
		return ;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("[mini$HELL]: cd: getcwd");
		return ;
	}
	update_env_var_value(&data->env_list, "PWD", cwd);
	free(cwd);
}
