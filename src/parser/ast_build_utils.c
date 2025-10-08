#include "minishell.h"

bool	is_redir_operator(t_operator_type op_type)
{
	return (op_type == OP_INPUT
		|| op_type == OP_OUTPUT
		|| op_type == OP_APPEND
		|| op_type == OP_HEREDOC);
}

bool	has_redirections(t_token *start, t_token *end)
{
	t_token	*curr;

	curr = start;
	while (curr && curr != end)
	{
		if(is_redir_operator(curr->op_type))
			return (true);
		curr = curr->next;
	}
	return (false);
}

bool	is_redir_filename(t_token *start, t_token *end, t_token *target)
{
	t_token	*curr;

	curr = start;
	while (curr && curr != end)
	{
		// if the token is a REDIR and the next is our TARGET
		if (is_redir_operator(curr->op_type) && curr->next == target)
			return (true);
		curr = curr->next;
	}
	return (false);
}

int	count_command_words(t_token *start, t_token *end)
{
	int		count;
	t_token	*curr;

	curr = start;
	count = 0;
	while (curr && curr != end)
	{
		if (!is_redir_filename(start, end , curr))
			count++;
		curr = curr->next;
	}
	return (count);
}

// free the tree using recursion
// TODO: make helper function to free strings + argv
void	free_ast(t_ast *node)
{
	t_ast	*curr;
	t_ast	*next;

	if (!node)
		return ;
	if (node->value)
		free(node->value);
	if (node->filename)
		free(node->filename);
	if (node->argv)
		free_strings_array(node->argv);
	if (node->left) // recursively free the left subtree
		free_ast(node->left);
	if (node->type == NODE_PIPE)
	{
		if (node->right)
			free_ast(node->right);
	}
	else if (node->type == NODE_CMD)
	{
		curr = node->right;
		while (curr)
		{
			next = curr->next;
			free_ast(curr);
			curr = next;
		}
	}
	free(node);
}
