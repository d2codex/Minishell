/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_trimming.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 18:05:39 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/15 18:05:40 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Process quotes using sequential state machine logic.
 *
 * Processes quotes character by character, following bash behavior for
 * quote removal and segment concatenation. Handles complex cases where
 * quoted and unquoted segments are adjacent within a single token.
 *
 * Quote processing rules:
 * - Single quotes preserve everything literally until closing quote
 * - Double quotes preserve everything literally until closing quote
 * - Adjacent quoted/unquoted segments are concatenated
 * - Quote characters themselves are consumed (not included in output)
 *
 * @param str Input string containing potentially mixed quoted/unquoted segments
 * @return Newly allocated string with quotes processed and segments
 * concatenated, or NULL on malloc failure. Empty quotes result in empty string.
 *
 * @note Uses the same quote state machine as the tokenizer for consistency.
 *       Caller is responsible for freeing the returned string.
 *
 * Examples:
 *   trim_quotes("\"hello\"")    → "hello" (outer quotes)
 *   trim_quotes("he\"llo\"")    → "hello" (internal quotes)
 *   trim_quotes("\"a\"'b'")     → "ab" (mixed quotes)
 *   trim_quotes("\"a b\"c")     → "a bc" (quote + unquoted)
 *   trim_quotes("\"\"")         → "" (empty quotes)
 */
char	*trim_quotes(const char *str)
{
	char	*result;
	int		i;
	int		j;
	t_quote	state;
	t_quote	new_state;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	state = STATE_NOT_IN_QUOTE;
	while (str[i])
	{
		new_state = update_quote_state(state, str[i]);
		if (new_state != state)
			state = new_state;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

/**
 * @brief Remove quotes from TOKEN_WORD nodes in the token list.
 *
 * Iterates over the token list and removes surrounding quotes from
 * tokens of type TOKEN_WORD, preserving inner content. This should
 * be done after variable expansion and before building the AST, so
 * that AST nodes get already-processed values.
 *
 * Example:
 * - Token value "\"hello\"" becomes "hello"
 * - Token value "'$HOME'" remains "$HOME" if quotes are single
 *
 * @param token_list Head of the token list to process
 * @return EXIT_SUCCESS if all tokens are successfully trimmed,
 *         EXIT_FAILURE if any allocation fails
 *
 * @note Modifies tokens in place. On failure, some tokens may be
 * partially processed; caller is responsible for cleanup.
 */
int	trim_quotes_in_token_list(t_token *token_list)
{
	t_token	*current;
	char	*trimmed;

	if (!token_list)
		return (EXIT_SUCCESS);
	current = token_list;
	while (current)
	{
		if (current->value && current->type == TOKEN_WORD)
		{
			trimmed = trim_quotes(current->value);
			if (!trimmed)
				return (EXIT_FAILURE);
			free(current->value);
			current->value = trimmed;
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
