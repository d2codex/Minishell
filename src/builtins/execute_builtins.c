#include "minishell.h"

/**
 * @brief Execute builtin commands.
 *
 * I'm not sure it will work like this !! But I need something to show you how
 * I imagined error code propagation from here
 *
 * @param tokens Tokens array from the command line
 * @param data Shell data structure
 * @return -1 for exit signal, 0 for continue
 */
int	execute_builtin(char **tokens, t_shell *data)
{
	(void)data;
	// TODO: Implement builtin lookup
	// TODO: Call appropriate builtin function
	// TODO: Handle return codes and status updates

	// return values: -1 = exit shell, 0 = continue execution
	// data->status stores actual error codes for the system

	// For now: just print the command
	printf("fakely executing this line: %s\n", tokens[0]);

	// fake update of the status
	data->status = 0;
	return (0);
}
