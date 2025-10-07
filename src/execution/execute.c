#include "minishell.h"

// temporary stub until execute_external_command is implemented
int	execute_external_command(char **tokens, t_shell *data)
{
	(void)tokens;
	(void)data;
	printf("TODO: execute externals\n");
	// just indicate command not found for now
	return (127); // CMD_NOT_FOUND_ERROR
}

int	execute_ast_tree(t_ast *node, t_shell *data)
{
	if (!node)
		return (EXIT_SUCCESS);
	if (node->type == NODE_PIPE)
	{
		// TODO: implement pipeline execution (fork, pipe, exec left and right)
		printf("TODO: Execute pipeline\n");
		return (EXIT_SUCCESS);
	}
	else if (node->type == NODE_CMD)
	{
		// TODO: Handle redirections in node->right first
		if (execute_builtin(node, data))
			return (data->status);
		//TODO: execute external command using node->argv
		return (execute_external_command(node->argv, data));
	}
	return (EXIT_SUCCESS);
}
