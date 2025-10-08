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
//t_token *find_first_pipe(t_token *tokens) TODO:DELETE
t_token	*find_first_pipe(t_token *start, t_token *end)
{
	t_token	*curr;

	//curr = tokens;TODO:DELETE
	curr = start;
	// while (curr) TODO: DELETE
	// conditions now include  end pointer, which is necessary since
	// we're no longer using next, there's no way to detect NULL;
	// instead the end pointer tells us when to stop.
	while (curr && curr != end)
	{
		if (curr->op_type == OP_PIPE)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

/* TODO: DELETE ENTIRELY
t_token	*split_at_pipe(t_token *tokens, t_token *pipe_token)
{
	t_token	*curr;
	t_token *right_subtree;

	curr = tokens;
	// split the token BEFORE the pipe and break the chain
	while (curr && curr->next != pipe_token)
		curr = curr->next;
	if (curr)
		curr->next = NULL; //break the chain before the pipe
	//return tokens AFTER the pipe (the right side)
	return (right_subtree);
}*/

//t_ast	*build_simple_command(t_token *tokens) TODO:DELETE
t_ast	*build_simple_command(t_token *start, t_token *end)
{
	char	**argv;
	t_ast	*cmd_node;
	t_ast	*redir_head;

//	if (!tokens) TODO: DELETE
	if (!start || start == end)
		return (NULL);
//	argv = collect_argv(tokens); TODO:DELETE
	argv = collect_argv(start, end);
	if (!argv)
		return (NULL);
	cmd_node = NULL;
	// create cmd_node if we have arguments
	if (argv && argv[0]) // check if there is at least one arg
	{
		cmd_node = create_cmd_node(argv);
		{
			if (!cmd_node)
				return (free_strings_array(argv), NULL);
		}
	}
	// collect all redirections into a chain
//	redir_head = collect_redirections(tokens); TODO:delete
	redir_head = collect_redirections(start, end);
	if (!redir_head && has_redirections(start, end))
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

static t_ast	*build_ast_recursive(t_token *start, t_token *end)
{
	t_token	*pipe_token;
	t_ast	*left_subtree;
	t_ast	*right_subtree;
	t_ast	*pipe_node;

	// ADDED: base case empty list
	if (!start || start == end)
		return (NULL);
	
	// pipe_token = find_first_pipe(tokens); TODO: DELETE!
	// find left most pipe in the segment
	// the segment now has clear start and end points.
	pipe_token = find_first_pipe(start, end);

	// if no pipe found build simple cmd
	if (!pipe_token)
		return (build_simple_command(start, end));

	// right_tokens = split_at_pipe(tokens, pipe_token); TODO: DELETE!
	// this is where it's breaking the tokens list making it impossible
	// for us to free the tokens list entirely resulting in leaks
	// for all nodes right of the pipe. we no longer need this

	//recursively build subtrees
	left_subtree = build_simple_command(start, pipe_token);
	if (!left_subtree)
		return (NULL);


	// DELETE: right_subtree = build_ast_from_tokens(right_tokens);
	
	
	right_subtree = build_ast_recursive(pipe_token->next, end);
	if (!right_subtree)
		return (free_ast(left_subtree), NULL);
	// now that the tree is no longer broken, we can start the right
	// subtree at pipe_token->next until end pointer.


	pipe_node = create_pipe_node(left_subtree, right_subtree);
	if (!pipe_node)
	{
		free_ast(left_subtree);
		free_ast(right_subtree);
		return (NULL);
	}
	return (pipe_node);
}

// new wrapper function, since there is now a start and end,
// the wrapper function is necessary to update those pointers.
t_ast *build_ast_from_tokens(t_token *tokens)
{
	// here we set the start and end tokens.
	// start: tokens, end: NULL
	return  (build_ast_recursive(tokens, NULL));
}
