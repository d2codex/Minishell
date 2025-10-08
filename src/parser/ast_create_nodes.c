#include "minishell.h"

char	**collect_argv(t_token *start, t_token *end)
{
	int		i;
	int		count;
	char	**argv;
	t_token	*curr;

	count = count_command_words(start, end);
	argv = malloc(sizeof (char *) * (count + 1));
	if (!argv)
		return (NULL);
	i = 0;
	curr = start;
	while (curr && curr != end)
	{
		if (curr->type == TOKEN_WORD && !is_redir_filename(start, end, curr))
		{
			argv[i] = ft_strdup(curr->value);
			if (!argv[i])
			{
				free_strings_array(argv);
				return (NULL);
			}
			i++;
		}
		curr = curr->next;
	}
	argv[i] = NULL;
	return (argv);
}

t_ast	*collect_redirections(t_token *start, t_token *end)
{
	t_ast	*redir_head;
	t_ast	*redir_tail;
	t_ast	*redir;
	t_token	*curr;

	redir_head = NULL;
	redir_tail = NULL;
	curr = start;
	while (curr && curr != end)
	{
		if (is_redir_operator(curr->op_type))
		{
			//make sure we dont go out of bounds
			if (!curr->next || curr->next == end)
				break ;
			// create redir n ode with operator and filename
			redir = create_redir_node(curr, curr->next);
			if (!redir)
				return (free_ast(redir_head), NULL); // free previously allocated nodes
			// add to chain
			if (!redir_head)
				redir_head = redir;
			else
				redir_tail->next = redir;
			redir_tail = redir;
			curr = curr->next; // skip file name token
		}
		curr = curr->next;
	}
	return (redir_head);
}

t_ast	*create_redir_node(t_token *op_token, t_token *file_token)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NODE_REDIR;
	node->op_type = op_token->op_type;
	node->value = ft_strdup(op_token->value);
	if (!node->value)
		return (free(node), NULL);
	node->argv = NULL;
	node->filename = ft_strdup(file_token->value);
	if (!node->filename)
		return (free(node->value), free(node), NULL);
	node->left = NULL;
	node->right = NULL;
	node->next = NULL;
	return (node);
}

t_ast	*create_cmd_node(char **argv)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NODE_CMD;
	node->op_type = OP_NONE;
	node->value = ft_strdup(argv[0]);
	if (!node->value)
		return (free(node), NULL);
	node->argv = argv;
	node->filename = NULL;
	node->left = NULL;
	node->right = NULL;
	node->next = NULL;
	return (node);
}

t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NODE_PIPE;
	node->op_type = OP_PIPE;
	node->value = ft_strdup("|"); //dup the string for easier freeing w/ recursion
	if (!node->value)
		return (free(node), NULL);
	node->argv = NULL;
	node->filename = NULL;
	node->left = left;
	node->right = right;
	node->next = NULL;
	return (node);
}
