#include "minishell.h"

/**
 * @brief Execute builtin commands.
 *
 * Looks up and executes the appropriate builtin command from the builtin table.
 * Handles different return code semantics:
 * - exit: returns its result directly (-1 for shell exit, 0 for continue)
 * - other builtins: their return code becomes data->status,
 * always continue shell
 *
 * @param tokens Tokens array from the command line where tokens[0] is the
 * command
 * @param data Shell data structure
 * @return -1 for exit signal (exit builtin only), 0 to continue shell execution
 */
int	execute_builtin(char **tokens, t_shell *data)
{
	int						i;
	int						result;
	static const t_builtin	builtins[] = {
	{"pwd", builtin_pwd},
	{"export", builtin_export},
	{"exit", builtin_exit},
	{"echo", builtin_echo},
	{"env", builtin_env},
	// unset
	// cd
	{NULL, NULL}};

	i = 0;
	while (builtins[i].cmd != NULL)
	{
		if (ft_strcmp(tokens[0], builtins[i].cmd) == 0)
		{
			result = builtins[i].f(tokens, data);
			if (ft_strcmp(tokens[0], "exit") == 0)
				return (result);
			data->status = result;
			return (0);
		}
		i++;
	}
	// cmd not found, set data->status = 127 ? (just a reminder for later)
	return (0);
}
