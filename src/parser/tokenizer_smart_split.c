#include "minishell.h"

/**
 * @brief Extract a special operator token (<, >, <<, >> and single |)
 * from the string.
 *
 * Only extracts if we're in STATE_NORMAL (not in quotes).
 * For mandatory part: treats << and >> as double tokens, || as two separate
 * | tokens. (if we want || to be interpreted as a double char for the bonus, 
 * we would need another implementation)
 *
 * Note: Recalculate quote state from start of string due to 42 norm
 * limitation (max 4 function parameters could not pass the enum as a 5th param)
 *
 * Helper for: extract_tokens_to_tab()
 *
 * @param s The string to parse
 * @param tab Array of strings to store tokens
 * @param i Pointer to current index in the string
 * @param token Pointer to current token count
 * @return TOKEN_OK on success, TOKEN_MALLOC_ERROR if allocation fails,
 * TOKEN_NOT_OPERATOR if not an operator in current state
 */
static t_token_error	extract_operator(const char *s, char **tab,
	size_t *i, size_t *token)
{
	t_quote	quote_state;
	size_t	j;
	size_t	len;

	quote_state = STATE_NORMAL;
	j = 0;
	while (j < *i)
	{
		quote_state = update_quote_state(quote_state, s[j]);
		j++;
	}
	if (quote_state != STATE_NORMAL
		|| (s[*i] != '|' && s[*i] != '<' && s[*i] != '>'))
		return (TOKEN_NOT_OPERATOR);
	len = 1;
	if ((s[*i] == '<' || s[*i] == '>') && s[*i + 1] == s[*i])
		len = 2;
	tab[*token] = ft_substr(s, *i, len);
	if (!tab[*token])
		return (free_string_array(tab, *token), TOKEN_MALLOC_ERROR);
	*i += len;
	(*token)++;
	return (TOKEN_OK);
}

/**
 * @brief Extract a word token (handles quoted substrings).
 *
 * Extracts one word from the string, taking into account quote states.
 * Processes characters until a separator is found, respecting quote boundaries.
 * Handles both single and double quotes, preserving metacharacters
 * inside quotes.
 *
 * Helper for: extract_tokens_to_tab()
 *
 * @param s Input string
 * @param tab Array of tokens
 * @param i Pointer to index in the string
 * @param token Pointer to current token count
 * @return TOKEN_OK on success, TOKEN_MALLOC_ERROR on memory allocation failure
 */
static t_token_error	extract_word(const char *s, char **tab, size_t *i,
	size_t *token)
{
	size_t	start;
	t_quote	current_quote_state;

	start = *i;
	current_quote_state = STATE_NORMAL;
	while (s[*i] && !is_a_shell_separator(current_quote_state, s[*i]))
	{
		current_quote_state = update_quote_state(current_quote_state, s[*i]);
		(*i)++;
	}
	if (*i > start)
	{
		tab[*token] = ft_substr(s, start, *i - start);
		if (!tab[*token])
		{
			free_string_array(tab, *token);
			return (TOKEN_MALLOC_ERROR);
		}
		(*token)++;
	}
	return (TOKEN_OK);
}

/**
 * @brief Check if an operator was successfully extracted.
 *
 * Helper for: extract_tokens_to_tab()
 *
 * @param error_code The error code returned by extract_operator
 * @return true if operator was extracted, false otherwise
 */
static bool	is_operator_extracted(t_token_error error_code)
{
	if (error_code == TOKEN_OK)
		return (true);
	return (false);
}

/**
 * @brief Fill the given array with tokens extracted from the string.
 *
 * Main orchestration function that coordinates token extraction.
 * Uses helper functions and maintains strict synchronization with
 * count_shell_tokens() to ensure consistent tokenization behavior.
 * note: is_operator_extracted is used to bypass the 25l limitation
 *
 * Relies on: extract_operator(), extract_word(), skip_whitespace(),
 * update_quote_state(), is_operator_extracted()
 *
 * @param s Input string
 * @param tab Preallocated token array
 * @return TOKEN_OK on success, TOKEN_MALLOC_ERROR if allocation failed
 */
/* static t_token_error	extract_tokens_to_tab(const char *s, char **tab)
{
	size_t			i;
	size_t			token;
	t_quote			quote_state;
	t_token_error	returned_code;
	int				token_counted;

	i = 0;
	token = 0;
	quote_state = STATE_NORMAL;
	while (s[i])
	{
		skip_whitespace(s, &i);
		if (!s[i])
			break ;
		quote_state = update_quote_state(quote_state, s[i]);
		returned_code = extract_operator(s, tab, &i, &token);
		if (returned_code == TOKEN_MALLOC_ERROR)
			return (returned_code);
		token_counted = is_operator_extracted(returned_code);
		if (token_counted == 0)
			returned_code = extract_word(s, tab, &i, &token);
		if (returned_code != TOKEN_OK
			&& returned_code != TOKEN_NOT_OPERATOR)
			return (returned_code);
	}
	return (tab[token] = NULL, TOKEN_OK);
} */
static t_token_error	extract_tokens_to_tab(const char *s, char **tab)
{
	size_t			i;
	size_t			token;
	t_quote			quote_state;
	t_token_error	code;

	i = 0;
	token = 0;
	quote_state = STATE_NORMAL;
	while (s[i])
	{
		skip_whitespace(s, &i);
		if (!s[i])
			break ;
		quote_state = update_quote_state(quote_state, s[i]);
		code = extract_operator(s, tab, &i, &token);
		if (code == TOKEN_MALLOC_ERROR)
			return (code);
		if (!is_operator_extracted(code))
			code = extract_word(s, tab, &i, &token);
		if (code != TOKEN_OK && code != TOKEN_NOT_OPERATOR)
			return (code);
	}
	tab[token] = NULL;
	return (TOKEN_OK);
}


/**
 * @brief Allocate array and split shell input into tokens.
 *
 * Checks for unclosed quotes before splitting.
 * On error, sets the provided error_code and returns NULL.
 *
 * ex of interface function that will call ft_split_token and use its returned
 * error code :
 *
 * t_token_error returned_code;
 * char **tokens;
 * tokens = ft_split_tokens(line, &returned_code);
 * if (returned_code == TOKEN_OK)
 *			=> advance to the next step
 * if (!tokens)
 * {
 *		if (returned_code == TOKEN_UNCLOSED_QUOTE)
 *			=> caller prints message about quotes and exit or return to prompt
 *		else if (returned_code == TOKEN_MALLOC_ERROR)
 *			=> caller prints perror("malloc") and exit
 *	}
 * @param s The input command line string
 * @param error_code Pointer to store error code (set to TOKEN_OK on success)
 * @return Array of tokens (NULL terminated) or NULL on failure
 */
char	**ft_split_tokens(char const *s, t_token_error *error_code)
{
	size_t			num_of_tokens;
	char			**final_tokens_tab;
	t_token_error	returned_code;

	if (has_unclosed_quotes(s))
	{
		*error_code = TOKEN_UNCLOSED_QUOTE;
		return (NULL);
	}
	num_of_tokens = count_shell_tokens(s);
	final_tokens_tab = ft_calloc((num_of_tokens + 1), sizeof(char *));
	if (!final_tokens_tab)
	{
		*error_code = TOKEN_MALLOC_ERROR;
		return (NULL);
	}
	returned_code = extract_tokens_to_tab(s, final_tokens_tab);
	if (returned_code != TOKEN_OK)
	{
		*error_code = returned_code;
		free(final_tokens_tab);
		return (NULL);
	}
	*error_code = TOKEN_OK;
	return (final_tokens_tab);
}
