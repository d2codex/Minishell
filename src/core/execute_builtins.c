#include "minishell.h"
//if this is illegal here we can put it inside the actual execute function
//which is ugly but whatever
static const t_builtin	builtins[] = {
{"pwd", builtin_pwd},
//add more here as it grows
{NULL, NULL}
};

/**
 * @brief Execute builtin commands.
 *
 * Placeholder function trying to demonstrate the new error code propagation system.
 * NOTE: I added a simple look up function, but that will change when we take
 * into account if is_child.  We can test any builtins that are in the parent.
 *
 * @param tokens Tokens array from the command line
 * @param data Shell data structure
 * @return -1 for exit signal, 0 for continue
 */
int	execute_builtin(char **tokens, t_shell *data)
{
	int	i;

	i = 0;
	while (builtins[i].cmd != NULL)
	{
		if (strcmp(tokens[0], builtins[i].cmd) == 0)
		{
			// TODO: Handle return codes and status updates **DONE**
			// data->status stores actual error codes for the system
			data->status = builtins[i].f(tokens, data);
			// return values: -1 = exit shell, 0 = continue execution
			if (strcmp(tokens[0], "exit") == 0)
				return (-1);
			return (0);
		}
		i++;
	}
	return (0);
}
