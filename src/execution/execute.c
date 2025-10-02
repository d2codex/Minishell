#include "minishell.h"

// temporary stub until execute_external_command is implemented
int	execute_external_command(char **tokens, t_shell *data)
{
	(void)tokens;
	(void)data;
	// just indicate command not found for now
	return (127); // CMD_NOT_FOUND_ERROR
}
