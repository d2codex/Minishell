#include "minishell.h"

/**
 * @brief Remove outer quotes from a string
 *
 * Only removes quotes if:
 * - String starts with ' or "
 * - String ends with the SAME quote character
 * - String length > 1
 *
 * Examples:
 *   "hello"  → hello
 *   'world'  → world
 *   hello	→ hello (no quotes, return copy)
 *   "hello'  → "hello' (mismatched, return copy)
 *
 * @param str Input string
 * @return Newly allocated string without outer quotes, or NULL on error
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
		// case where you have "" or ''
		if (str_len == 2)
			return (ft_strdup(""));
		trimmed_len = str_len - 2;
		// malloc done internally into substring
		trimmed_str = ft_substr(str, 1, trimmed_len);
	}
	else
		trimmed_str = ft_strdup(str);
	return (trimmed_str);
}

/**
 * @brief Remove quotes from all relevant AST nodes
 *
 * Processes NODE_CMD and NODE_REDIR values, removing outer quotes
 * while preserving inner content. Skips operators (NODE_PIPE). If someone puts
 * quote around an operator, it means they want to use the string and not the
 * operator.
 *
 * Memory management: Frees old node->value and replaces with trimmed version.
 * If trimming fails (malloc error), leaves node unchanged.
 *
 * @param ast_list Head of flat AST list to process
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

