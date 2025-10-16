/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 18:02:39 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/15 18:02:40 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Find the first pipe token in a token range.
 *
 * Scans the tokens from `start` up to (but not including) `end`
 * and returns the first token representing a pipe (`|`).
 *
 * @param start Pointer to the first token in the range.
 * @param end Pointer to the token marking the end of the range (excluded).
 * @return Pointer to the first pipe token, or NULL if none found.
 */
t_token	*find_first_pipe(t_token *start, t_token *end)
{
	t_token	*curr;

	curr = start;
	while (curr && curr != end)
	{
		if (curr->op_type == OP_PIPE)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

/**
 * @brief Free command resources safely.
 *
 * Frees either a command node or its argv array, depending on what
 * was allocated.
 *
 * @param cmd_node Command AST node to free (can be NULL).
 * @param argv Argument array to free if no node exists.
 * @return Always returns NULL for convenience in cleanup chains.
 */
static t_ast	*cleanup_command(t_ast *cmd_node, char **argv)
{
	if (cmd_node)
		free_ast(cmd_node);
	else if (argv)
		free_strings_array(argv);
	return (NULL);
}

/**
 * @brief Build a command or redirection node sequence from tokens.
 *
 * Constructs a command node with its argv and attaches any redirections.
 * Supports commands without redirections, redirections without commands,
 * or both. Returns NULL only on allocation or structural failure.
 *
 * @param start Start of the token range for the command.
 * @param end End of the token range (excluded).
 * @return A fully built command or redirection node chain, or NULL on error.
 */
t_ast	*build_simple_command(t_token *start, t_token *end)
{
	char	**argv;
	t_ast	*cmd_node;
	t_ast	*redir_head;

	if (!start || start == end)
		return (NULL);
	argv = collect_argv(start, end);
	cmd_node = NULL;
	if (argv && argv[0])
	{
		cmd_node = create_cmd_node(argv);
		if (!cmd_node)
			return (free_strings_array(argv), NULL);
	}
	redir_head = collect_redirections(start, end);
	if (!redir_head && has_redirections(start, end))
		return (cleanup_command(cmd_node, argv));
	if (cmd_node && redir_head)
		cmd_node->right = redir_head;
	if (cmd_node)
		return (cmd_node);
	if (redir_head)
		return (redir_head);
	return (NULL);
}

/**
 * @brief Recursively build an AST from a token range.
 *
 * Splits tokens by the first pipe symbol and builds left/right subtrees.
 * Base case: a simple command with optional redirections.
 *
 * @param start Start of token range.
 * @param end End of token range (NULL for full list).
 * @return Root of the constructed AST, or NULL on failure.
 */
static t_ast	*build_ast_recursive(t_token *start, t_token *end)
{
	t_token	*pipe_token;
	t_ast	*left_subtree;
	t_ast	*right_subtree;
	t_ast	*pipe_node;

	if (!start || start == end)
		return (NULL);
	pipe_token = find_first_pipe(start, end);
	if (!pipe_token)
		return (build_simple_command(start, end));
	left_subtree = build_simple_command(start, pipe_token);
	if (!left_subtree)
		return (NULL);
	right_subtree = build_ast_recursive(pipe_token->next, end);
	if (!right_subtree)
		return (free_ast(left_subtree), NULL);
	pipe_node = create_pipe_node(left_subtree, right_subtree);
	if (!pipe_node)
	{
		free_ast(left_subtree);
		free_ast(right_subtree);
		return (NULL);
	}
	return (pipe_node);
}

/**
 * @brief Entry point for AST construction.
 *
 * Builds a full AST from the given token list.
 *
 * @param tokens Head of the token list.
 * @return Root of the AST, or NULL on failure.
 */
t_ast	*build_ast_from_tokens(t_token *tokens)
{
	return (build_ast_recursive(tokens, NULL));
}
