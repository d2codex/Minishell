/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_build_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 18:02:32 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/15 18:02:33 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if an operator type is a redirection.
 *
 * @param op_type The operator type to check.
 * @return true if it is OP_INPUT, OP_OUTPUT, OP_APPEND, or OP_HEREDOC;
 * false otherwise.
 */
bool	is_redir_operator(t_operator_type op_type)
{
	return (op_type == OP_INPUT
		|| op_type == OP_OUTPUT
		|| op_type == OP_APPEND
		|| op_type == OP_HEREDOC);
}

/**
 * @brief Check if a token range contains any redirection operators.
 *
 * @param start Pointer to the first token in the range.
 * @param end Pointer to the token marking the end of the range (excluded).
 * @return true if any token in the range is a redirection operator;
 * false otherwise.
 */
bool	has_redirections(t_token *start, t_token *end)
{
	t_token	*curr;

	curr = start;
	while (curr && curr != end)
	{
		if (is_redir_operator(curr->op_type))
			return (true);
		curr = curr->next;
	}
	return (false);
}

/**
 * @brief Check if a token is a filename for a redirection.
 *
 * Scans the token range from start to end (excluded) to see if
 * `target` follows a redirection operator.
 *
 * @param start Pointer to the first token in the range.
 * @param end Pointer to the token marking the end of the range (excluded).
 * @param target Token to check as a redirection target.
 * @return true if target is a redirection filename; false otherwise.
 */
bool	is_redir_filename(t_token *start, t_token *end, t_token *target)
{
	t_token	*curr;

	curr = start;
	while (curr && curr != end)
	{
		if (is_redir_operator(curr->op_type) && curr->next == target)
			return (true);
		curr = curr->next;
	}
	return (false);
}

/**
 * @brief Count command words within a token range.
 *
 * Iterates through the given token range and counts tokens of type
 * `TOKEN_WORD`, excluding those used as redirection filenames.
 *
 * @param start Pointer to the first token in the range.
 * @param end   Pointer to the token marking the end of the range (excluded).
 * @return The number of valid command words found.
 */
int	count_command_words(t_token *start, t_token *end)
{
	int		count;
	t_token	*curr;

	curr = start;
	count = 0;
	while (curr && curr != end)
	{
		if (curr->type == TOKEN_WORD && !is_redir_filename(start, end, curr))
			count++;
		curr = curr->next;
	}
	return (count);
}
