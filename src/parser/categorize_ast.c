#include "minishell.h"

/**
 * @brief Free a flat AST node list.
 *
 * Iterates over the AST list using the `next` pointer and frees each node's
 * dynamically allocated members:
 * - the `value` string (owned by the AST node),
 * - the `argv` array (the array itself; strings inside are aliases of `value`),
 * - `filename` is **not freed** here if it points to `value`.
 *
 * The AST node itself is freed after its members.
 *
 * This function assumes the list is "flat" (not a tree). The `left` and
 * `right` pointers may be set but are ignored during traversal.
 *
 * @param list Head of the AST list to free. Safe to pass `NULL`.
 */
void	free_ast_list(t_ast *list)
{
	t_ast	*current;

	while (list)
	{
		current = list;
		list = list->next;
		free(current->argv);
		if (current->value)
			free(current->value);
		free(current);
	}
}

/**
 * @brief Allocate and initialize a new AST node from a token.
 *
 * Creates a new AST node with all pointers initialized to NULL and
 * `type` set to NODE_NONE. If the token has a `value`, it is duplicated
 * into the node. The `op_type` is copied directly from the token.
 *
 * @note This function only initializes the node; fields such as
 *       `argv`, `filename`, `left`, and `right` will be set or
 *       updated in later stages of AST construction.
 *
 * @param token Pointer to the token to base the AST node on. If NULL,
 *              the function returns NULL immediately.
 * @return t_ast* Pointer to the newly allocated AST node, or NULL on
 *         allocation failure.
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
	new->value = NULL;
	if (token->value)
	{
		new->value = ft_strdup(token->value);
		if (!new->value)
			return (free(new), NULL);
	}
	new->argv = NULL;
	new->filename = NULL;
	new->op_type = token->op_type;
	new->left = NULL;
	new->right = NULL;
	new->next = NULL;
	return (new);
}

/**
 * @brief Check if an operator type represents a redirection.
 *
 * Recognized redirection operators are: <, >, >>, and <<.
 *
 * @param op_type Operator type to check.
 * @return true if the operator is a redirection, false otherwise.
 */
bool	is_a_redirection(t_operator_type op_type)
{
	if (op_type == OP_INPUT || op_type == OP_OUTPUT
		|| op_type == OP_APPEND || op_type == OP_HEREDOC)
		return (true);
	return (false);
}

/**
 * @brief Assign node types in the AST based on token position and operator type.
 *
 * Performs a first pass over the flat AST list to classify nodes:
 * - The first node in the list, or the first node following a pipe, is marked
 *   as a command (`NODE_CMD`).
 * - Pipe operators (`|`) are marked as `NODE_PIPE`, and the following node
 *   will be treated as the start of a new command.
 * - Redirection operators (`<`, `>`, `>>`, `<<`) are marked as `NODE_REDIR`.
 * - Any other nodes remain `NODE_NONE` for now.
 *
 * This function modifies the AST nodes in place and does not return a value.
 *
 * @param ast_list Head of the AST linked list. Safe to pass NULL.
 */
void	assign_ast_node_type(t_ast *ast_list)
{
	t_ast	*current;
	int		new_command;

	if (!ast_list)
		return ;
	new_command = 1;
	current = ast_list;
	while (current)
	{
		if (is_a_redirection(current->op_type))
			current->type = NODE_REDIR;
		else if (current->op_type == OP_PIPE)
		{
			current->type = NODE_PIPE;
			new_command = 1;
		}
		else if (new_command)
		{
			current->type = NODE_CMD;
			new_command = 0;
		}
		current = current->next;
	}
}

/**
 * @brief Create a flat doubly linked AST node list from a token list.
 *
 * Iterates over the token list and converts each token into an AST node.
 * The nodes are linked sequentially:
 * - `next` and `right` point forward to the next node,
 * - `left` points back to the previous node.
 *
 * A tail pointer is used to append nodes in O(1).
 * If allocation of a node fails, the partially built AST list is freed
 * and `NULL` is returned.
 *
 * @param tokens_list Pointer to the head of the token list.
 * @return Head of the new AST list, or `NULL` on error.
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
			return (free_ast_list(ast_list), NULL);
		if (!ast_list)
			ast_list = new;
		else
		{
			tail->next = new;
			tail->right = new;
			new->left = tail;
		}
		tail = new;
		current = current->next;
	}
	return (ast_list);
}
