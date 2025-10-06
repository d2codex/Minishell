#include "minishell.h"

/**
 * @brief Print a syntax error message and return MISUSAGE_ERROR.
 *
 * Prints a shell-style error message:
 * - Uses the token's value if available.
 * - Prints "newline" if the error is at the end of input.
 *
 * Example:
 * ```
 * bash: syntax error near unexpected token `|`
 * bash: syntax error near unexpected token `newline`
 * ```
 *
 * @param token Token that caused the syntax error (may be NULL).
 * @return Always returns MISUSAGE_ERROR.
 */
static int	syntax_error(t_token *token)
{
	ft_putstr_fd(ERR_PREFIX, STDERR_FILENO);
	ft_putstr_fd(ERR_SYNTAX, STDERR_FILENO);
	if (token && token->value)
		ft_putstr_fd(token->value, STDERR_FILENO);
	else
		ft_putstr_fd("newline", STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	return (MISUSAGE_ERROR);
}

/**
 * @brief Check if a token is a redirection operator.
 *
 * @param token Token to check.
 * @return true if it's a redirection, false otherwise.
 */
static bool	is_token_redirection(t_token *token)
{
	if (!token || token->type != TOKEN_WORD)
		return (false);
	return (token->op_type == OP_REDIR_IN
		|| token->op_type == OP_REDIR_OUT
		|| token->op_type == OP_APPEND
		|| token->op_type == OP_HEREDOC);
}

/**
 * @brief Validate the first token in the linear list.
 *
 * The first token determines whether the input starts with valid syntax.
 * Rules enforced:
 * - Empty input (no tokens) is valid.
 * - The first token cannot be a pipe.
 * - A leading redirection is only valid if it has a proper target (WORD) after it.
 * - Otherwise, the first token must be a WORD (typically a command).
 *
 * @param curr Pointer to the first token in the list.
 * @return EXIT_SUCCESS if the first token is valid,
 *         MISUSAGE_ERROR if it violates shell syntax rules.
 *
 * On error, a descriptive syntax error message is printed to stderr.
 */
static int	validate_first_token(t_token *curr)
{
	if (!curr)
		return (EXIT_SUCCESS); //empty input - not error
	if (curr->type == TOKEN_WORD) // first node must be a CMD
		return (EXIT_SUCCESS);
	if (curr->type == OP_PIPE)
		return (syntax_error(curr));
	if (is_token_redirection(curr))
	{
		if (!curr->next || curr->next->type != TOKEN_WORD)
			return (syntax_error(curr->right));
		return (syntax_error(NULL));
	}
	return (syntax_error(NULL));
}

/**
 * @brief Validate the syntax of a linear list of parsed tokens.
 *
 * This function ensures that the linked list of parsed tokens follows
 * basic shell grammar rules regarding the ordering of pipes and redirections.
 * The list is still linear (flat), not yet expanded into an AST.
 *
 * **Rules enforced:**
 * - The first token must be valid (checked by validate_first_token()).
 * - **Pipes:**
 *   - Cannot appear at the start or end of the input.
 *   - Cannot appear consecutively (e.g., `cmd | | cmd`).
 * - **Redirections:**
 *   - Must be followed by a valid WORD token (i.e., a filename or argument).
 *   - Cannot be followed by another operator (pipe or redirection).
 * - The final token cannot be a pipe or a redirection operator.
 *
 * @param list Pointer to the head of the token list.
 * @return
 * - `EXIT_SUCCESS` if the syntax is valid.
 * - `MISUSAGE_ERROR` if a syntax error is detected.
 *
 * On error, a descriptive message is printed to `stderr`:
 * ```
 * minishell: syntax error near unexpected token `|`
 * minishell: syntax error near unexpected token `newline`
 * ```
 */
int	validate_syntax_token_list(t_token *list)
{
	t_ast	*curr;

	if (validate_first_token(list) != EXIT_SUCCESS)
		return (MISUSAGE_ERROR);
	curr = list;
	while (curr && curr->next)
	{
		// two pipes in a row
		if (curr->op_type == OP_PIPE && curr->next->op_type == OP_PIPE)
			return (syntax_error(curr->next));
		// redirection without a valid target
		if (is_token_redirection(curr))
		{
			if (!curr->next || curr->next->type != TOKEN_WORD)
				return (syntax_error(curr->next));
		}
		curr = curr->next;
	}
	//last token can't be pipe or redirection
	if (curr && (curr->op_type == OP_PIPE || is_token_redirection(curr)))
		return (syntax_error(NULL));
	return (EXIT_SUCCESS);
}
