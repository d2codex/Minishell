#include "minishell.h"

static void	print_indent(int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
}

static void	print_node_type(t_node_type type)
{
	if (type == NODE_CMD)
		printf("NODE_CMD");
	else if (type == NODE_PIPE)
		printf("NODE_PIPE");
	else if (type == NODE_REDIR)
		printf("NODE_REDIR");
	else
		printf("NODE_NONE");
}

static void	print_op_type(t_operator_type op_type)
{
	if (op_type == OP_PIPE)
		printf("OP_PIPE");
	else if (op_type == OP_INPUT)
		printf("OP_INPUT");
	else if (op_type == OP_OUTPUT)
		printf("OP_OUTPUT");
	else if (op_type == OP_APPEND)
		printf("OP_APPEND");
	else if (op_type == OP_HEREDOC)
		printf("OP_HEREDOC");
	else
		printf("OP_NONE");
}

static void	print_argv(char **argv)
{
	int	i;

	if (!argv)
	{
		printf("NULL");
		return;
	}
	printf("[");
	i = 0;
	while (argv[i])
	{
		printf("\"%s\"", argv[i]);
		if (argv[i + 1])
			printf(", ");
		i++;
	}
	printf("]");
}

void	print_ast(t_ast *node, int depth)
{
	t_ast	*redir;

	if (!node)
	{
		print_indent(depth);
		printf("(NULL)\n");
		return;
	}

	print_indent(depth);
	printf("┌─ Node @ %p\n", (void *)node);

	print_indent(depth);
	printf("│  Type: ");
	print_node_type(node->type);
	printf("\n");

	print_indent(depth);
	printf("│  OpType: ");
	print_op_type(node->op_type);
	printf("\n");

	print_indent(depth);
	printf("│  Value: %s\n", node->value ? node->value : "NULL");

	print_indent(depth);
	printf("│  Argv: ");
	print_argv(node->argv);
	printf("\n");

	print_indent(depth);
	printf("│  Filename: %s\n", node->filename ? node->filename : "NULL");

	print_indent(depth);
	printf("│  heredoc_fd: %d\n", node->heredoc_fd);
	// Print redirections chain if this is a CMD node
	if (node->type == NODE_CMD && node->right)
	{
		print_indent(depth);
		printf("│  Redirections:\n");
		redir = node->right;
		while (redir)
		{
			print_indent(depth + 1);
			printf("├─ ");
			print_op_type(redir->op_type);
			printf(" -> %s\n", redir->filename ? redir->filename : "NULL");
			redir = redir->next;
		}
	}

	// Print left subtree
	if (node->left)
	{
		print_indent(depth);
		printf("│  Left:\n");
		print_ast(node->left, depth + 1);
	}

	// Print right subtree (only for PIPE nodes)
	if (node->type == NODE_PIPE && node->right)
	{
		print_indent(depth);
		printf("│  Right:\n");
		print_ast(node->right, depth + 1);
	}

	print_indent(depth);
	printf("└─ End Node @ %p\n", (void *)node);
}
