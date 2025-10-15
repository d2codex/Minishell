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
void	cleanup_node(t_ast *node)
{
	if (node->value)
		free(node->value);
	if (node->filename)
		free(node->filename);
	if (node->argv)
		free_strings_array(node->argv);
	// Close heredoc FD if still valid
	if (node->heredoc_fd >= 0)
		close_fds(&node->heredoc_fd);
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
	// free nodes string resources and fds
	cleanup_node(node);
	free(node);
}
