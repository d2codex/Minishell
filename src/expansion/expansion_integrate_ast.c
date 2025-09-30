#include "minishell.h"

/**
 * @brief Synchronize token array with expanded AST values.
 *
 * This function bridges the gap between the expanded AST and the legacy token
 * array used by execute_builtin() and execute_external_command(). After variable
 * expansion is applied to the AST, this function copies the expanded values back
 * to the original token array to maintain compatibility with existing execution
 * functions.
 *
 * The function iterates through both the AST list and token array simultaneously,
 * replacing each token string with the corresponding expanded value from the AST.
 * Memory management is handled properly - old token strings are freed before
 * being replaced with newly allocated copies of the expanded values.
 *
 * @param tokens Array of token strings to update with expanded values
 * @param ast_list Flat AST list containing expanded variable values
 *
 * @note This is a temporary bridge function until execution migrates to AST-based
 * @note Old token strings are freed and replaced with new allocations
 * @note The AST values are copied, not moved - AST retains its own memory
 * @note Token array must be null-terminated and match AST node count
 *
 * Example:
 *   Before: tokens = ["echo", "$USER"]
 *   After:  tokens = ["echo", "john"] (assuming USER=john)
 */
void	sync_tokens_with_ast(char **tokens, t_ast *ast_list)
{
	t_ast	*current;
	int		i;

	current = ast_list;
	i = 0;
	while (current && tokens[i])
	{
		if (current->value)
		{
			free(tokens[i]);
			tokens[i] = ft_strdup(current->value);
		}
		current = current->next;
		i++;
	}
}

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
			|| current->type == NODE_REDIR || current->type == NODE_NONE))
		{
			expanded = expand_variables_in_string(current->value, data);
			if (expanded)
			{
				free(current->value); // free previous string
				current->value = expanded; // replace by the brand new one
			}
		}
		current = current->next;
	}
}
