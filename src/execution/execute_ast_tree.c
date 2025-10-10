#include "minishell.h"

/**
 * @brief Execute an AST node (command or pipeline).
 *
 * Recursively executes the abstract syntax tree built from the parsed
 * command line. Handles both simple commands and pipelines (TODO).
 * For simple commands, checks if it's a builtin first, otherwise
 * executes as an external command.
 *
 * @param node AST node to execute (NODE_CMD or NODE_PIPE)
 * @param data Shell data structure
 * @return Exit status of the executed command
 *
 * @note Redirections (node->right) are not yet implemented (TODO).
 * @note Pipeline execution is not yet implemented (TODO).
 */
int	execute_ast_tree(t_ast *node, t_shell *data)
{
	if (!node)
		return (EXIT_SUCCESS);
	if (node->type == NODE_PIPE)
		return (execute_pipeline(node, data));
	else if (node->type == NODE_CMD)
	{
		// TODO: Handle redirections in node->right first
		// Check if it's a builtin command
		if (execute_builtin(node, data))
		{
		// Builtin executed successfully
		// In child process (pipeline): must exit, not return
		// Note: 'exit' builtin handles is_child internally and exits before returning here
		if (data->is_child)
			exit(data->status);
		// In parent: return to display next prompt
		return (data->status);
		}
		// Not a builtin: execute external command (forks internally)
		return (execute_external_command(node->argv, data));
	}
	return (EXIT_SUCCESS);
}
