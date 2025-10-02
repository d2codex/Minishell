#include "minishell.h"

/**
 * @brief Validate tokenization result
 *
 * Checks whether the tokenizer returned a valid array of tokens.
 * This function does not free the tokens or the input line;
 * all cleanup must be handled separately (e.g., in cleanup_line).
 *
 * @param tokens The array of token strings returned by the tokenizer
 * @return true if tokens are valid (non-NULL and non-empty), false otherwise
 */
bool	validate_tokens(char **tokens)
{
	if (!tokens)
		return (false);
	if (!tokens[0])
		return (false);
	return (true);
}

/**
 * @brief Tokenize input and handle tokenization errors.
 * Helper for process_line()
 *
 * @param line Input line to tokenize
 * @param data Shell data for error status updates
 * @return Tokenized array or NULL on error
 */
char	**execute_tokenizer(char *line, t_shell *data)
{
	t_token_error	error_code;
	char			**tokens;

	tokens = ft_split_tokens(line, &error_code);
	if (error_code != TOKEN_OK)
	{
		if (error_code == TOKEN_UNCLOSED_QUOTE)
		{
			print_error(ERR_PREFIX, "syntax error: unclosed quotes",
				NULL, NULL);
			data->status = 2;
		}
		else if (error_code == TOKEN_MALLOC_ERROR)
		{
			print_error(ERR_PREFIX, "memory allocation failed", NULL, NULL);
			data->status = 1;
		}
		return (NULL);
	}
	return (tokens);
}
