#include "minishell.h"

/**
 * @brief Counts the number of arguments in a command AST sequence.
 *
 * Starts from a NODE_CMD node and counts:
 * - The command itself (`cmd_node->value`).
 * - Any immediately following NODE_NONE nodes (plain arguments).
 *
 * @param cmd_node Pointer to an AST node of type NODE_CMD.
 *
 * @return
 * - Number of arguments (>= 1, since the command itself is always counted).
 */
static int	count_cmd_args(t_ast *cmd_node)
{
	int		count;
	t_ast	*curr;

	count = 1;
	curr = cmd_node->next;
	while (curr && curr->type == NODE_NONE)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

/**
 * @brief Collects command arguments from an AST command node.
 *
 * Builds a NULL-terminated `argv` array for a command node, starting with
 * the command itself and followed by its plain argument nodes.
 *
 * @param cmd_node Pointer to an AST node of type NODE_CMD.
 *
 * @return
 * - A dynamically allocated NULL-terminated array of argument strings.
 * - NULL if memory allocation fails.
 *
 * @note
 * - The returned array must be freed by the caller.
 * - The strings in `argv` are not duplicated — they point to the `value`
 *   fields of AST nodes. Their lifetime is managed by the AST.
 * - Only consecutive NODE_NONE nodes are treated as arguments.
 */
static char	**collect_argv(t_ast *cmd_node)
{
	int		i;
	int		argc;
	char	**argv;
	t_ast	*curr;

	argc = count_cmd_args(cmd_node);
	argv = malloc(sizeof (char *) * (argc + 1));
	if (!argv)
		return (NULL);
	argv[0] = cmd_node->value;
	curr = cmd_node->next;
	i = 1;
	while (curr && curr->type == NODE_NONE)
	{
		argv[i] = curr->value;
		i++;
		curr = curr->next;
	}
	argv[i] = NULL;
	return (argv);
}

/**
 * @brief Assigns argv arrays and filenames to AST nodes.
 *
 * This function iterates through the AST list and performs the following:
 * - For nodes of type NODE_CMD, it collects command arguments into
 *   the `argv` array using collect_argv().
 * - For nodes of type NODE_REDIR, it assigns the filename pointer
 *   from the following node's `value`.
 *
 * @param ast_list Pointer to the head of the AST linked list.
 *
 * @return
 * - 0 on success.
 * - EXIT_FAILURE if memory allocation fails (e.g., collect_argv()).
 * - MISUSAGE_ERROR if a redirection node is missing a filename
 *   (syntax error).
 *
 * @note
 * - On error, the AST list should be freed by the caller
 *   (e.g., using free_ast_list()).
 * - This function does not duplicate `filename` strings, it simply
 *   points to the value of the next node. The lifetime of these
 *   strings must be managed elsewhere.
 */
int	assign_argv_and_filename(t_ast *ast_list)
{
	t_ast	*curr;

	curr = ast_list;
	while (curr)
	{
		if (curr->type == NODE_CMD)
		{
			curr->argv = collect_argv(curr);
			if (!curr->argv)
				return (EXIT_FAILURE);
		}
		else if (curr->type == NODE_REDIR)
		{
			if (curr->op_type == OP_HEREDOC)
				curr->filename = curr->next->value;
			else
				curr->filename = curr->next->value;
			curr = curr->next;
		}
		curr = curr->next;
	}
	return (EXIT_SUCCESS);
}
