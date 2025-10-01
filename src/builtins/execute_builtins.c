#include "minishell.h"

/**
 * @brief Check and execute a builtin command.
 *
 * Compares the first token against the list of supported builtins,
 * including `exit`. If a match is found, executes the corresponding
 * function and updates `data->status`. Does not execute external commands.
 *
 * @param argv Command argv from user input.
 * @param data Shell state, including environment, exit status, and exit flag.
 * @return true if the command is a builtin and was executed; false otherwise.
 */
bool	execute_builtin(t_ast *list, t_shell *data)
{
	int						i;
	static const t_builtin	builtins[] = {
	{"pwd", builtin_pwd}, {"export", builtin_export},
	{"exit", builtin_exit}, {"echo", builtin_echo},
	{"env", builtin_env}, {"unset", builtin_unset},
	{"cd", builtin_cd},
	{NULL, NULL}};

	if (!list || !list->argv[0])
		return (false);
	i = 0;
	while (builtins[i].cmd != NULL)
	{
		if (ft_strcmp(list->value, builtins[i].cmd) == 0)
		{
			builtins[i].f(list->argv, data);
			return (true);
		}
		i++;
	}
	return (false);
}
