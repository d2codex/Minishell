/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <diade-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:21:04 by diade-so          #+#    #+#             */
/*   Updated: 2025/10/07 16:36:44 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*find_first_pipe(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	while (curr)
	{
		if (curr->op_type == OP_PIPE)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

t_token	*split_at_pipe(t_token *tokens, t_token *pipe_token)
{
	t_token	*curr;

	curr = tokens;
	// split the token BEFORE the pipe and break the chain
	while (curr && curr->next != pipe_token)
		curr = curr->next;
	if (curr)
		curr->next = NULL; //break the chain before the pipe
	//return tokens AFTER the pipe (the right side)
	return (pipe_token->next);
}

t_ast	*build_simple_command(t_token *tokens)
{
	char	**argv;
	t_ast	*cmd_node;
	t_ast	*redir_head;

	if (!tokens)
		return (NULL);
	argv = collect_argv(tokens);
	if (!argv)
		return (NULL);
	cmd_node = NULL;
	// create cmd_node if we have arguments
	if (argv && argv[0]) // check if there is at least one arg
	{
		cmd_node = create_cmd_node(tokens->value, argv);
		{
			if (!cmd_node)
				return (free_strings_array(argv), NULL);
		}
	}
	// collect all redirections into a chain
	redir_head = collect_redirections(tokens);
	if (!redir_head && has_redirections(tokens))
	{
		if (cmd_node)
			free_ast(cmd_node);
		else
			free_strings_array(argv);
		return (NULL);
	}
	if (cmd_node)
	{
		cmd_node->right = redir_head;
		return (cmd_node);
	}
	return (redir_head);
}

t_ast	*build_ast_from_tokens(t_token *tokens)
{
	t_token	*pipe_token;
	t_token	*right_tokens;
	t_ast	*right_subtree;
	t_ast	*left_subtree;
	t_ast	*pipe_node;

	if (!tokens)
		return (NULL);
	pipe_token = find_first_pipe(tokens);
	if (!pipe_token)
		return (build_simple_command(tokens));
	right_tokens = split_at_pipe(tokens, pipe_token);
	//recursively build subtrees
	left_subtree = build_simple_command(tokens);
	if (!left_subtree)
		return (NULL); //handle error
	right_subtree = build_ast_from_tokens(right_tokens);
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
