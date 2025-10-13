#include "minishell.h"

/**
 * @brief Determine whether a command should be executed in a separate process.
 *
 * Non-forking builtins (like cd, export, unset, exit) are executed in the parent
 * process to affect shell state. All other commands, including external commands
 * and forkable builtins, should be executed in a child process.
 *
 * @param node AST node containing the command.
 * @param data Pointer to the shell state (currently unused but may be needed later).
 * @return true if the command should be executed in a child process; false otherwise.
 */
bool	should_fork(t_ast *node, t_shell *data)
{
	(void)data;
	if (!node || node->type != NODE_CMD)
		return (false);
	if (is_builtin(node))
	{
		if (is_nonforking_builtin(node))
			return (false);
	}
	return (true);
}

/**
 * @brief Check if the AST node represents a builtin command.
 *
 * @param node AST node containing the command.
 * @return true if the command is a builtin (pwd, export, exit, echo, env, unset, cd);
 *         false otherwise.
 */
bool	is_builtin(t_ast *node)
{
	int					i;
	static const char	*builtins[] = {
		"pwd", "export", "exit", "echo", "env", "unset", "cd", NULL};

	if (!node || !node->value)
		return (false);
	i = 0;
	while (builtins[i] != NULL)
	{
		if (ft_strcmp(node->value, builtins[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}

/**
 * @brief Check if a builtin command should run without forking.
 *
 * Some builtins need to run in the parent process to modify shell state directly.
 *
 * @param node AST node containing the command.
 * @return true if the builtin is non-forking (cd, export, unset, exit); false otherwise.
 */
bool	is_nonforking_builtin(t_ast *node)
{
	int					i;
	static const char	*non_forking[] = {
		"cd", "export", "unset", "exit", NULL};

	if (!node || !node->value)
		return (false);
	i = 0;
	while (non_forking[i] != NULL)
	{
		if (ft_strcmp(node->value, non_forking[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}
