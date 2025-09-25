#include "minishell.h"

/**
 * @brief Free a flat AST node list.
 *
 * Iterates over a singly linked list of AST nodes and frees each node's
 * dynamically allocated members (`args` array and `filename`) before freeing
 * the node itself. Assumes that the `left` and `right` pointers are NULL
 * (i.e., this is a flat list, not a tree).
 *
 * @param list Head of the AST list to free. Safe to pass NULL.
 */
void	free_ast_list(t_ast *list)
{
	t_ast	*current;

	while (list)
	{
		current = list;
		list = list->next;
		free_strings_array(current->args);
		if (current->filename)
			free (current->filename);
		free(current);
	}
}

/**
 * @brief Allocate and initialize a new AST node from a token.
 *
 * Creates a new AST node with all pointers initialized to NULL and
 * `type` set to NODE_NONE. Copies the `op_type` from the given token.
 *
 * @note This function only initializes the node; its other values
 *       (like `args`, `filename`, `left`, `right`) will be set
 *       or updated in later passes as the AST is built.
 *
 * @param token Pointer to the token to base the AST node on. Must not be NULL.
 * @return t_ast* Pointer to the newly allocated AST node, or NULL on allocation failure.
 */
t_ast	*new_ast_node(t_token *token)
{
	t_ast	*new;

	if (!token)
		return (NULL);
	new = malloc(sizeof (t_ast));
	if (!new)
		return (NULL);
	new->type = NODE_NONE;
	new->args = NULL;
	new->filename = NULL;
	new->op_type = token->op_type;
	new->left = NULL;
	new->right = NULL;
	new->next = NULL;
	return (new);
}

//now loop through and update the fields depending on the position
//of the nodes

/**
 * @brief Create a flat AST node list from a token list.
 *
 * Iterates over the token list and converts each token into an AST node.
 * If any allocation fails, the partially built AST list is freed and NULL
 * is returned. Uses a tail pointer to append nodes in O(1).
 *
 * @param tokens_list Pointer to the head of the token list.
 * @return t_ast* Head of the new AST list, or NULL on error.
 */
t_ast	*create_ast_list(t_token *tokens_list)
{
	t_ast	*new;
	t_ast	*ast_list;
	t_ast	*tail;
	t_token	*current;

	ast_list = NULL;
	tail = NULL;
	current = tokens_list;
	while (current)
	{
		new = new_ast_node(current);
		if (!new)
		{
			free_ast_list(ast_list);
			return (NULL);
		}
		if (!ast_list) //no list yet (first node of the list)
			ast_list = new;
		else
			tail->next = new;
		tail = new; // update tail to the last node
		current = current->next;
	}
	return (ast_list);
}
