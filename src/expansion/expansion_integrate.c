#include "minishell.h"

/**
 * @brief Apply variable expansion to all relevant nodes in the AST list.
 *
 * Iterates through the flat AST list and expands environment variables
 * in node values where appropriate. Only processes command nodes and
 * redirection filenames, leaving operators unchanged.
 *
 * @param ast_list Head of the flat AST list to process
 * @param data Shell data structure containing environment variables
 *
 * @note Modifies AST nodes in place, replacing node->value with expanded version
 * @note Frees old values and allocates new ones - caller doesn't need cleanup
 */
void	expand_ast_nodes(t_ast *ast_list, t_shell *data)
{
	t_ast	*current;
	char	*expanded;

	if (!ast_list || !data)
		return ;
	current = ast_list;
	while (current)
	{
		// Expand variables in commands and redirection filenames
		if (current->value && (current->type == NODE_CMD
			|| current->type == NODE_REDIR))
		{
			expanded = expand_variables_in_string(current->value, data);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		current = current->next;
	}
}
