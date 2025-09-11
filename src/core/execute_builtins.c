#include "minishell.h"

/**
 * @brief Execute builtin commands.
 *
 * Placeholder function trying to demonstrate the new error code propagation system.
 * Will be extended with actual builtin lookup and execution.
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
	printf("I am a fake command, I just return your line: %s\n", tokens[0]);

	// fake update of the status
	data->status = 0;
	return (0);
}
