#include "minishell.h"

/**
 * @brief Test printing of AST nodes.
 *
 * This test uses the output from `categorize_tokens` (a linked list of t_token)
 * to create a flat AST list. It then prints all fields of each AST node,
 * including `args`, `filename`, `left`, `right`, `type`, and `op_type`.
 *
 * Only the `t_ast` struct fields are output; no execution or evaluation
 * of commands is performed.
 */
void	test_ast_tokens(void)
{
	char	*tokens[] = {
		"echo", ">", "-n", "<<", "hello", ">>", "|", "wc", "<", NULL};
	t_token	*token_list;
	t_ast	*ast_list;
	t_ast	*current;
	char	*op_str;
	int		i;

	printf(YEL "----- TOKENS LIST -----\n" RESET);
	i = 0;
	while (tokens[i])
	{
		printf("%s[%d]%s %s%s%s ", CYN, i, RESET, YEL, tokens[i], RESET);
		i++;
	}
	printf(YEL "\n--- Testing AST creation ---\n" RESET);
	// Create token list first
	token_list = create_token_type_list(tokens);
	if (!token_list)
	{
		printf(YEL "Failed to create token list\n" RESET);
		return ;
	}
	// Create AST list from token list
	ast_list = create_ast_list(token_list);
	if (!ast_list)
	{
		printf(YEL "Failed to create AST list\n" RESET);
		free_tokens_list(token_list);
		return ;
	}
	current = ast_list;
	int j = 0;
	while (current)
	{
		if (current->op_type == OP_PIPE)
			op_str = "OP_PIPE";
		else if (current->op_type == OP_INPUT)
			op_str = "OP_INPUT";
		else if (current->op_type == OP_OUTPUT)
			op_str = "OP_OUTPUT";
		else if (current->op_type == OP_APPEND)
			op_str = "OP_APPEND";
		else if (current->op_type == OP_HEREDOC)
			op_str = "OP_HEREDOC";
		else
			op_str = "OP_NONE";
		printf(CYN "[%d] AST Node at %p\n" RESET, j, (void *)current);
		printf("         type: %d\n", current->type);
		printf("      op_type: %s\n", op_str);
		if (current->args)
		{
			printf("        args:\n");
			i = 0;
			while (current->args[i])
			{
				printf("    [%d] (%p): %s\n", i, (void *)current->args[i], current->args[i]);
				i++;
			}
		}
		else
			printf("         args: NULL\n");
		if (current->filename)
			printf("     filename: %s (%p)\n", current->filename, (void *)current->filename);
		else
			printf("     filename: NULL\n");
		printf("         left: %p\n", (void *)current->left);
		printf("        right: %p\n", (void *)current->right);
		current = current->next;
		printf("\n");
		j++;
	}
	free_ast_list(ast_list);
	free_tokens_list(token_list);
	printf(YEL "--- AST test complete ---\n\n" RESET);
}

int	main(void)
{
	test_ast_tokens();
	return (0);
}
