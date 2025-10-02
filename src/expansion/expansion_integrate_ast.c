#include "minishell.h"

/**
 * @brief Apply variable expansion to all relevant nodes in the AST list.
 *
 * Iterates through the flat AST list and expands environment variables
 * in node values where appropriate. Only processes command nodes and
 * redirection filenames, leaving operators unchanged. If any expansion
 * fails due to malloc error, stops processing and returns failure.
 *
 * @param ast_list Head of the flat AST list to process
 * @param data Shell data structure containing environment variables
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on malloc error
 *
 * @note Modifies AST nodes in place, replacing node->value with expanded 
 * version
 * @note Frees old values and allocates new ones - caller must check return 
 * value
 * @note On failure, some nodes may be partially expanded, cleanup handled by 
 * caller
 */
int	expand_ast_nodes(t_ast *ast_list, t_shell *data)
{
	t_ast	*current;
	char	*expanded;

	if (!ast_list)
		return (EXIT_SUCCESS);
	if (!data)
		return (EXIT_FAILURE);
	current = ast_list;
	while (current)
	{
		if (current->value && (current->type == NODE_CMD
				|| current->type == NODE_NONE))
		{
			expanded = expand_variables_in_string(current->value, data);
			if (!expanded)
				return (EXIT_FAILURE);
			free(current->value);
			current->value = expanded;
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
