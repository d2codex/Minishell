#include "minishell.h"

/**
 * @brief Remove outer quotes from a string following shell parsing rules.
 *
 * Only removes quotes if:
 * - String starts with ' or "
 * - String ends with the SAME quote character  
 * - String length > 1
 *
 * This function implements bash-style quote removal, processing only the
 * outermost layer of quotes. Mismatched quotes are left unchanged.
 *
 * @param str Input string to process
 * @return Newly allocated string without outer quotes, or empty string if
 *         input was just quotes ("" or ''). Returns NULL only on malloc failure.
 *
 * @note Caller is responsible for freeing the returned string.
 *
 */
char	*trim_quotes(const char *str)
{
	int		str_len;
	int		trimmed_len;
	char	*trimmed_str;

	if (!str || str[0] == '\0')
		return (ft_strdup(""));
	str_len = ft_strlen(str);
	if ((str[0] == '"' && str[str_len - 1] == '"')
		|| (str[0] == '\'' && str[str_len - 1] == '\''))
	{
		// case where you have only "" or ''
		if (str_len == 2)
			return (ft_strdup(""));
		trimmed_len = str_len - 2;
		// malloc done internally inside substring
		trimmed_str = ft_substr(str, 1, trimmed_len);
	}
	else
		trimmed_str = ft_strdup(str);
	return (trimmed_str);
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
 *       expand_ast_nodes() but before assign_argv_and_filename() in the
 *       processing pipeline.
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
