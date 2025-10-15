/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:40:55 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/15 17:40:56 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Execute a shell builtin command if it matches a known builtin.
 *
 * This function checks the command in the given AST node against the list
 * of supported builtin commands (`pwd`, `export`, `exit`, `echo`, `env`,
 * `unset`, `cd`). If a match is found, the corresponding function is executed,
 * and the shell state (`data->status`) is updated accordingly.
 *
 * This function does **not** handle external commands; it only executes
 * builtins.
 *
 * @param node Pointer to the AST node representing the command.
 * @param data Pointer to the shell state structure, which includes environment,
 *             exit status, and other runtime flags.
 * @return int The updated exit status after executing the builtin, or
 *             EXIT_FAILURE if the command is not a builtin or the node is
 * invalid.
 */
int	execute_builtin(t_ast *node, t_shell *data)
{
	int						i;
	static const t_builtin	builtins[] = {
	{"pwd", builtin_pwd}, {"export", builtin_export},
	{"exit", builtin_exit}, {"echo", builtin_echo},
	{"env", builtin_env}, {"unset", builtin_unset},
	{"cd", builtin_cd},
	{NULL, NULL}};

	if (!node || !node->value)
		return (EXIT_FAILURE);
	if (data->curr_ast && data->is_child)
		close_all_heredocs(data->curr_ast);
	i = 0;
	while (builtins[i].cmd != NULL)
	{
		if (ft_strcmp(node->value, builtins[i].cmd) == 0)
		{
			builtins[i].f(node->argv, data);
			return (data->status);
		}
		i++;
	}
	return (EXIT_FAILURE);
}
