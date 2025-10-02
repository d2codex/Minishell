#include "minishell.h"

/**
 * @brief Process quotes using sequential state machine logic.
 *
 * Processes quotes character by character, following bash behavior for
 * quote removal and segment concatenation. Handles complex cases where
 * quoted and unquoted segments are adjacent within a single token.
 *
 * Quote processing rules:
 * - Single quotes preserve everything literally until closing quote
 * - Double quotes preserve everything literally until closing quote  
 * - Adjacent quoted/unquoted segments are concatenated
 * - Quote characters themselves are consumed (not included in output)
 *
 * @param str Input string containing potentially mixed quoted/unquoted segments
 * @return Newly allocated string with quotes processed and segments
 * concatenated, or NULL on malloc failure. Empty quotes result in empty string.
 *
 * @note Uses the same quote state machine as the tokenizer for consistency.
 *       Caller is responsible for freeing the returned string.
 *
 * Examples:
 *   trim_quotes("\"hello\"")    → "hello" (outer quotes)
 *   trim_quotes("he\"llo\"")    → "hello" (internal quotes)  
 *   trim_quotes("\"a\"'b'")     → "ab" (mixed quotes)
 *   trim_quotes("\"a b\"c")     → "a bc" (quote + unquoted)
 *   trim_quotes("\"\"")         → "" (empty quotes)
 */
char	*trim_quotes(const char *str)
{
	char	*result;
	int		i;
	int		j;
	t_quote	state;
	t_quote	new_state;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	state = STATE_NOT_IN_QUOTE;
	while (str[i])
	{
		new_state = update_quote_state(state, str[i]);
		if (new_state != state)
			state = new_state;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

/**
 * @brief Remove quotes from command and argument nodes in the AST.
 *
 * Processes NODE_CMD and NODE_NONE values, removing outer quotes while
 * preserving inner content. This runs after variable expansion in the
 * pipeline to clean up quoted arguments before execution.
 *
 * Node types processed:
 * - NODE_CMD: Command names ("echo" → echo)
 * - NODE_NONE: Arguments ("hello world" → hello world)
 * - NODE_PIPE: Skipped (operators should not be quote-processed)
 * - NODE_REDIR: Skipped (handled separately in filename assignment)
 *
 * Memory management: Frees the old node->value and replaces it with the
 * trimmed version. If trimming fails due to malloc error, the node is
 * left unchanged to maintain system stability.
 *
 * @param ast_list Head of the flat AST list to process
 *
 * @note This function modifies the AST in place and should be called after
 *	   expand_ast_nodes() but before assign_argv_and_filename() in the
 *	   processing pipeline.
 */
void	trim_quotes_in_ast(t_ast *ast_list)
{
	t_ast	*current;
	char	*trimmed;

	if (!ast_list)
		return ;
	current = ast_list;
	while (current)
	{
		if (current->value && (current->type == NODE_CMD
				|| current->type == NODE_NONE))
		{
			trimmed = trim_quotes(current->value);
			if (trimmed != current->value)
			{
				free(current->value);
				current->value = trimmed;
			}
		}
		current = current->next;
	}
}
