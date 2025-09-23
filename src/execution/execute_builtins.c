#include "minishell.h"

/**
 * @brief Check and execute a builtin command.
 *
 * Compares the first token against the list of supported builtins, 
 * including `exit`. If a match is found, executes the corresponding 
 * function and updates `data->status`. Does not execute external commands.
 *
 * @param tokens Command tokens from user input.
 * @param data Shell state, including environment, exit status, and exit flag.
 * @return true if the command is a builtin and was executed; false otherwise.
 */
bool	execute_builtin(char **tokens, t_shell *data)
{
	int						i;
	static const t_builtin	builtins[] = {
	{"pwd", builtin_pwd}, {"export", builtin_export},
	{"exit", builtin_exit}, {"echo", builtin_echo},
	{"env", builtin_env}, {"unset", builtin_unset},
	// cd
	{NULL, NULL}};

	// no tokens at all, nothing to execute
	if (!tokens || !tokens[0])
		return (false); // not a builtin
	i = 0;
	while (builtins[i].cmd != NULL)
	{
		// compare user command agains each builtin keyword
		// including exit
		if (ft_strcmp(tokens[0], builtins[i].cmd) == 0)
		{
			builtins[i].f(tokens, data);
			return (true); //if true it's a builtin
		}
		i++;
	}
	return (false);
}
