#include "minishell.h"

/**
 * @brief Test function for AST creation and node type assignment.
 *
 * This function simulates parsing a shell command into tokens,
 * converts them into an AST (Abstract Syntax Tree), and then
 * assigns node types based on operator rules and position.
 *
 * It uses a hardcoded example command line:
 *   echo > -n << hello >> | wc <
 *
 * Steps performed:
 *  - Create a token list from the raw tokens (via create_token_type_list).
 *  - Convert the token list into an AST list (via create_ast_list).
 *  - Assign node types such as NODE_CMD, NODE_PIPE, NODE_REDIR
 *    depending on operator type and position (via assign_ast_node_type).
 *  - Print each AST node with its type, operator, arguments, filename,
 *    and child pointers for debugging purposes.
 *
 * The test only outputs the AST node structure; it does not execute
 * any commands.
 */
static const char	*node_type_to_str(t_node_type type)
{
	if (type == NODE_CMD)
		return ("NODE_CMD");
	else if (type == NODE_PIPE)
		return ("NODE_PIPE");
	else if (type == NODE_REDIR)
		return ("NODE_REDIR");
	return ("NODE_NONE");
}

static const char	*op_type_to_str(t_operator_type op_type)
{
	if (op_type == OP_PIPE)
		return ("OP_PIPE");
	else if (op_type == OP_INPUT)
		return ("OP_INPUT");
	else if (op_type == OP_OUTPUT)
		return ("OP_OUTPUT");
	else if (op_type == OP_APPEND)
		return ("OP_APPEND");
	else if (op_type == OP_HEREDOC)
		return ("OP_HEREDOC");
	return ("OP_NONE");
}

void	test_ast_tokens(void)
{
	char	*tokens[] = {
		"cat", "input.txt",        // command + arg (input file)
		"|", "grep", "pattern",    // pipe → new command + arg
		">", "out1.txt",           // output redirection
		">>", "out2.log",          // append redirection
		"<", "extra.txt",          // input redirection
		"<<", "EOF",               // heredoc redirection
		"|", "sort", "-r",         // another pipe command with flag
		"|", "head", "-n", "5",    // final pipe command with args
		NULL};
	t_token	*token_list;
	t_ast	*ast_list;
	t_ast	*current;
	int		i;
	int		j;

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
	assign_ast_node_type(ast_list);
	current = ast_list;
	j = 0;
	while (current)
	{
		printf(CYN "[%d] AST Node at %p\n" RESET, j, (void *)current);
		printf("        value: %s%s%s\n", YEL, current->value, RESET);
		printf("         type: %s\n", node_type_to_str(current->type));
		printf("      op_type: %s\n", op_type_to_str(current->op_type));
		if (current->argv)
		{
			printf("        argv:\n");
			i = 0;
			while (current->argv[i])
			{
				printf("    [%d] (%p): %s\n", i, (void *)current->argv[i], current->argv[i]);
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
