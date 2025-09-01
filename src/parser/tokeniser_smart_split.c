#include "minishell.h"

/**
 * @brief Extract a special operator token (<, >, <<, >>, |) from the string.
 *
 * Only extracts if we're in STATE_NORMAL (not in quotes).
 *
 * @param s The string to parse
 * @param tab Array of strings to store tokens
 * @param i Pointer to current index in the string
 * @param token Pointer to current token count
 * @param quote_state Current quote state
 * @return TOKEN_OK on success, TOKEN_MALLOC_ERROR if allocation fails,
 *         TOKEN_NOT_OPERATOR if not an operator in current state
 */
static t_token_error	extract_operator(const char *s, char **tab, size_t *i,
	size_t *token, t_quote quote_state)
{
	size_t	len;

	// Only extract as operator if we're not in quotes
	if (quote_state != STATE_NORMAL ||
		(s[*i] != '|' && s[*i] != '<' && s[*i] != '>'))
		return (TOKEN_NOT_OPERATOR);  // Pas un opérateur dans ce contexte

	len = 1;
	if ((s[*i] == '<' || s[*i] == '>') && s[*i + 1] == s[*i])
		len = 2;
	tab[*token] = ft_substr(s, *i, len);
	if (!tab[*token])
	{
		free_string_array(tab, *token);
		return (TOKEN_MALLOC_ERROR);
	}
	*i += len;
	(*token)++;
	return (TOKEN_OK);
}

/**
 * @brief Extract a word token (handles quoted substrings).
 *
 * Extracts one word from the string, taking into account quote states.
 * Processes characters until a separator is found, respecting quote boundaries.
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
 * @brief Fill the given array with tokens extracted from the string.
 *
 * Splits operators and words into separate tokens.
 * Each extraction function manages its own quote state tracking.
 *
 * @param s Input string
 * @param tab Preallocated token array
 * @return TOKEN_OK on success, TOKEN_MALLOC_ERROR if allocation failed
 */
static t_token_error	extract_tokens_to_tab(const char *s, char **tab)
{
	size_t			i;
	size_t			token;
	t_quote			quote_state;
	t_token_error	returned_error_code;
	int				tmp;

	i = 0;
	token = 0;
	quote_state = STATE_NORMAL;
	while (s[i])
	{
		skip_whitespace(s, &i);
		if (!s[i])
			break ;

		quote_state = update_quote_state(quote_state, s[i]);

		// Même logique que count_shell_tokens
		returned_error_code = extract_operator(s, tab, &i, &token, quote_state);
		if (returned_error_code == TOKEN_OK)
			tmp = 1;
		else
			tmp = 0;

		if (tmp == 0)
			returned_error_code = extract_word(s, tab, &i, &token);

		if (returned_error_code != TOKEN_OK && returned_error_code != TOKEN_NOT_OPERATOR)
			return (returned_error_code);
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
 * t_token_error returned_error_code;
 * char **tokens;
 * tokens = ft_split_tokens(line, &returned_error_code);
 * if (!tokens)
 * {
 *		if (returned_error_code == TOKEN_UNCLOSED_QUOTE)
 *			=> print some error and do whatever you have to do here
 *		else if (returned_error_code == TOKEN_MALLOC_ERROR)
 *			=> print some error and do whatever you have to do here
 *	}
 *
 * @param s The input command line string
 * @param error_code Pointer to store error code (set to TOKEN_OK on success)
 * @return Array of tokens (NULL terminated) or NULL on failure
 */
char	**ft_split_tokens(char const *s, t_token_error *error_code)
{
	size_t			num_of_tokens;
	char			**final_tokens_tab;
	t_token_error	returned_error_code;

	if (has_unclosed_quotes(s))
	{
		*error_code = TOKEN_UNCLOSED_QUOTE;
		print_error("$Hell: ", "unclosed quote detected");
		return (NULL);
	}
	num_of_tokens = count_shell_tokens(s);
	final_tokens_tab = ft_calloc((num_of_tokens + 1), sizeof(char *));
	if (!final_tokens_tab)
	{
		*error_code = TOKEN_MALLOC_ERROR;
		return (NULL);
	}
	returned_error_code = extract_tokens_to_tab(s, final_tokens_tab);
	if (returned_error_code != TOKEN_OK)
	{
		*error_code = returned_error_code;
		free(final_tokens_tab);
		return (NULL);
	}
	return (*error_code = TOKEN_OK, final_tokens_tab);
}
