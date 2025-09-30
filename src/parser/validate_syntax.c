#include "minishell.h"

/**
 * @brief Print a syntax error message to stderr and return MISUSAGE_ERROR.
 *
 * The function formats the error according to shell behavior:
 * - Prints a fixed error prefix and "syntax error near unexpected token".
 * - If a node and its value are provided, prints the token value.
 * - Otherwise, defaults to printing "newline".
 *
 * Example outputs:
 * ```
 * bash: syntax error near unexpected token `|`
 * bash: syntax error near unexpected token `newline`
 * ```
 *
 * @param node Pointer to the AST node that caused the syntax error
 *             (may be NULL if the error is at end of input).
 * @return Always returns MISUSAGE_ERROR.
 */
static int	syntax_error(t_ast *node)
{
	ft_putstr_fd(ERR_PREFIX, STDERR_FILENO);
	ft_putstr_fd(ERR_SYNTAX, STDERR_FILENO);
	if (node && node->value)
		ft_putstr_fd(node->value, STDERR_FILENO);
	else
		ft_putstr_fd("newline", STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	return (MISUSAGE_ERROR);
}

/**
 * @brief Validate the first node of the linear token list.
 *
 * The first node in the list has stricter syntax rules:
 * - Empty input (no node at all) is considered valid.
 * - The first node must be a command.
 * - A leading pipe is invalid.
 * - A leading redirection is only valid if followed by a proper target
 *   (not another operator). Otherwise, it's a syntax error.
 *
 * @param curr Pointer to the first node in the list.
 * @return EXIT_SUCCESS if valid,
 *         MISUSAGE_ERROR if the first node violates syntax rules.
 *
 * On error, a syntax error message is printed to stderr.
 */
static int	validate_first_node(t_ast *curr)
{
	if (!curr)
		return (EXIT_SUCCESS); //empty input - not error
	if (curr->type == NODE_CMD) // first node must be a CMD
		return (EXIT_SUCCESS);
	if (curr->type == NODE_PIPE)
		return (syntax_error(curr));
	if (curr->type == NODE_REDIR)
	{
		if (curr->right && (curr->right->type == NODE_PIPE ||
			curr->right->type == NODE_REDIR))
			return (syntax_error(curr->right));
		return (syntax_error(NULL));
	}
	return (syntax_error(NULL));
}

/**
 * @brief Validate the syntax of a linear list of parsed tokens.
 *
 * This function checks whether the linked list of parsed tokens (with node
 * types already assigned) follows valid shell grammar rules for pipes and
 * redirections. The list is still linear, not yet expanded into a full AST.
 *
 * Rules enforced:
 * - The first node must be a valid command (handled by validate_first_node()).
 * - Pipes:
 *   - Cannot appear at the start or end of the input.
 *   - Cannot be consecutive (e.g., cmd | | cmd).
 * - Redirections:
 *   - Must be followed by a valid target (WORD / NODE_NONE).
 *   - Cannot be followed by another operator (pipe or redirection).
 *
 * @param list The head of the token list to validate.
 * @return EXIT_SUCCESS if syntax is valid,
 *         MISUSAGE_ERROR if a syntax error is detected.
 *
 * On error, a descriptive syntax error message is printed to stderr.
 */
int	validate_syntax_ast_list(t_ast *list)
{
	t_ast	*curr;

	curr = list;
	if (validate_first_node(curr))
		return (MISUSAGE_ERROR);
	while (curr)
	{
		if (curr->type == NODE_PIPE)
		{
			if (!curr->right) // pipe at end → newline
				return (syntax_error(NULL));
			if (curr->right->type == NODE_PIPE)
				return (syntax_error(curr->right));
		}
		else if (curr->type == NODE_REDIR)
		{
			if (!curr->right)
				return (syntax_error(NULL));
			if (curr->right->type != NODE_CMD && curr->right->type != NODE_NONE)
				return (syntax_error(curr->right)); //checkes consecutive operators
		}
		curr = curr->right;
	}
	return (EXIT_SUCCESS);
}
