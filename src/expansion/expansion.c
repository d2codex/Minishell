#include "minishell.h"

/**
 * @brief Expand environment variables in a token list.
 *
 * Iterates over a linked list of `t_token` nodes and expands any
 * environment variables present in `TOKEN_WORD` nodes using the
 * shell context provided in `data`. Quotes are preserved at this
 * stage, only variable expansion is performed.
 *
 * Example:
 * - Input token value: "$HOME/Documents"
 * - Output token value: "/home/user/Documents"
 *
 * @param tokens_list Head of the token list to process.
 * @param data Shell context containing environment variables.
 * @return EXIT_SUCCESS if all expansions succeed,
 *         EXIT_FAILURE if an error occurs (e.g., memory allocation fails).
 */
int	expand_tokens_list(t_token *tokens_list, t_shell *data)
{
	t_token	*current;
	char	*expanded;

	if (!tokens_list)
		return (EXIT_SUCCESS);
	if (!data)
		return (EXIT_FAILURE);
	current = tokens_list;
	while (current)
	{
		if (current->value && current->type == TOKEN_WORD)
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
