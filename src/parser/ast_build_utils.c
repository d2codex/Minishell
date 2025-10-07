#include "minishell.h"

bool	is_redir_operator(t_operator_type op_type)
{
	return (op_type == OP_INPUT
		|| op_type == OP_OUTPUT
		|| op_type == OP_APPEND
		|| op_type == OP_HEREDOC);
}

bool	has_redirections(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	while (curr)
	{
		if(is_redir_operator(tokens->op_type))
			return (true);
		curr = curr->next;
	}
	return (false);
}

bool	is_redir_filename(t_token *tokens, t_token *target)
{
	t_token	*curr;

	curr = tokens;
	while (curr)
	{
		// if the token is a REDIR and the next is our TARGET
		if (is_redir_operator(curr->op_type) && curr->next == target)
			return (true);
		curr = curr->next;
	}
	return (false);
}

int	count_command_words(t_token *tokens)
{
	int		count;
	t_token	*curr;

	curr = tokens;
	count = 0;
	while (curr)
	{
		if (!is_redir_filename(tokens, curr))
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
