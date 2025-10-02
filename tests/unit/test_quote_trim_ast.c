#include "minishell.h"

char *trim_quotes(const char *str)
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
 * while preserving inner content. Skips operators (NODE_PIPE).
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

	 printf("DEBUG: trim_quotes_in_ast() START\n");
	if (!ast_list)
	{
		printf("DEBUG: ast_list is NULL!\n");
		return ;
	}

	current = ast_list;
	while (current)
	{
		printf("DEBUG: Processing node type=%d, value='%s'\n",
				current->type, current->value ? current->value : "NULL");
		if (current->value && (current->type == NODE_CMD
			|| current->type == NODE_REDIR))
		{
			printf("DEBUG: Eligible for trimming: '%s'\n", current->value);
			trimmed = trim_quotes(current->value);
			if (trimmed)
			{
				printf("DEBUG: Before trim: '%s' → After trim: '%s'\n",
					current->value, trimmed);
				free(current->value); // free previous string
				current->value = trimmed; // replace by the brand new one
			}
			else
				printf("DEBUG: trim_quotes returned NULL!\n");
		}
		else
			printf("DEBUG: Skipping node (not eligible)\n");
		current = current->next;
	}
	printf("DEBUG: trim_quotes_in_ast() END\n");
}

t_ast *create_test_node(t_node_type type, const char *value)
{
	t_ast *node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_ast));
	node->type = type;
	if (value)
		node->value = ft_strdup(value);
	node->next = NULL;
	return node;
}

int main(void)
{
	t_ast *list = NULL;
	t_ast *node1, *node2, *node3;

	// mini list AST : echo "hello" | grep 'world'
	node1 = create_test_node(NODE_CMD, "echo");
	node2 = create_test_node(NODE_CMD, "\"hello\""); // to trim
	node3 = create_test_node(NODE_PIPE, "|"); // to ignore

	node1->next = node2;
	node2->next = node3;
	list = node1;

	printf("BEFORE trim_quotes_in_ast:\n");
	printf("Node1: %s\n", node1->value);
	printf("Node2: %s\n", node2->value);
	printf("Node3: %s\n", node3->value);

	// snip snip
	trim_quotes_in_ast(list);

	printf("\nAFTER trim_quotes_in_ast:\n");
	printf("Node1: %s (should be: echo)\n", node1->value);
	printf("Node2: %s (should be: hello)\n", node2->value);
	printf("Node3: %s (should be: |)\n", node3->value);

	// not freeing that, yolo

	return 0;
}
