/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_create_nodes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 18:03:16 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/15 18:03:21 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Collect command arguments from a token range.
 *
 * Extracts all tokens of type TOKEN_WORD that are not redirection filenames
 * and returns them as a NULL-terminated array of strings.
 *
 * @param start Pointer to the first token in the range.
 * @param end Pointer to the token marking the end of the range (excluded).
 * @return NULL-terminated array of argument strings, or NULL on
 * allocation failure.
 */
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
				return (free_strings_array(argv), NULL);
			i++;
		}
		curr = curr->next;
	}
	argv[i] = NULL;
	return (argv);
}

/**
 * @brief Collect redirection nodes from a token range.
 *
 * Creates a linked list of redirection AST nodes for all redirection operators
 * in the token range.
 *
 * @param start Pointer to the first token in the range.
 * @param end Pointer to the token marking the end of the range (excluded).
 * @return Head of the redirection AST linked list, or NULL if none or
 * on failure.
 */
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
		if (is_redir_operator(curr->op_type) && curr->next && curr->next != end)
		{
			redir = create_redir_node(curr, curr->next);
			if (!redir)
				return (free_ast(redir_head), NULL);
			if (!redir_head)
				redir_head = redir;
			else
				redir_tail->next = redir;
			redir_tail = redir;
			curr = curr->next;
		}
		curr = curr->next;
	}
	return (redir_head);
}

/**
 * @brief Create a single redirection AST node.
 *
 * Uses an operator token and a filename token to initialize an AST
 * node of type NODE_REDIR.
 *
 * @param op_token Token representing the redirection operator.
 * @param file_token Token representing the filename for redirection.
 * @return Pointer to the new AST node, or NULL on allocation failure.
 */
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
	node->heredoc_fd = -1;
	node->left = NULL;
	node->right = NULL;
	node->next = NULL;
	return (node);
}

/**
 * @brief Create a command AST node.
 *
 * Initializes an AST node of type NODE_CMD using the provided argv array.
 *
 * @param argv NULL-terminated array of argument strings (argv[0] is the
 *             command name).
 * @return Pointer to the new AST node, or NULL on allocation failure.
 */
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
	node->heredoc_fd = -1;
	node->left = NULL;
	node->right = NULL;
	node->next = NULL;
	return (node);
}

/**
 * @brief Create a pipe AST node.
 *
 * Initializes an AST node of type NODE_PIPE connecting left and right child
 * nodes.
 *
 * @param left Left child AST node.
 * @param right Right child AST node.
 * @return Pointer to the new AST node, or NULL on allocation failure.
 */
t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NODE_PIPE;
	node->op_type = OP_PIPE;
	node->value = ft_strdup("|");
	if (!node->value)
		return (free(node), NULL);
	node->argv = NULL;
	node->filename = NULL;
	node->heredoc_fd = -1;
	node->left = left;
	node->right = right;
	node->next = NULL;
	return (node);
}
