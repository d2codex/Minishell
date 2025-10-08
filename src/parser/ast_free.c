#include "minishell.h"

/**
 * @brief Free the dynamically allocated strings in a single AST node.
 *
 * Frees the following fields if they are not NULL:
 *  - value
 *  - filename
 *  - argv (via free_strings_array)
 *
 * @param node Pointer to the AST node whose strings will be freed.
 */
void	free_strings_in_node(t_ast *node)
{
	if (node->value)
		free(node->value);
	if (node->filename)
		free(node->filename);
	if (node->argv)
		free_strings_array(node->argv);
}

/**
 * @brief Recursively free an entire AST.
 *
 * Frees all nodes in the AST including:
 *  - left and right subtrees
 *  - right-linked redirection nodes for commands
 *  - all strings in each node via free_strings_in_node
 *
 * @param node Pointer to the root AST node to free.
 */
void	free_ast(t_ast *node)
{
	t_ast	*curr;
	t_ast	*next;

	if (!node)
		return ;
	free_strings_in_node(node);
	if (node->left)
		free_ast(node->left);
	if (node->type == NODE_PIPE)
	{
		if (node->right)
			free_ast(node->right);
	}
	else if (node->type == NODE_CMD)
	{
		curr = node->right;
		while (curr)
		{
			next = curr->next;
			free_ast(curr);
			curr = next;
		}
	}
	free(node);
}
