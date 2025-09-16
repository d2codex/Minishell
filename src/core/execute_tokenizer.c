#include "minishell.h"

/**
 * @brief Validate tokenization result and handle cleanup on error
 *
 * @param tokens Result from tokenization
 * @param line Original input line
 * @return true if tokens are valid, false if error (cleanup already done)
 */
bool	validate_tokens(char **tokens, char *line)
{
	if (!tokens)
	{
		free(line);
		return (false);
	}
	if (!tokens[0])
	{
		free_string_array(tokens, 0);
		free(line);
		return (false);
	}
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
			print_error("[mini$Hell]: ", "syntax error: unclosed quotes");
			data->status = 2;
		}
		else if (error_code == TOKEN_MALLOC_ERROR)
		{
			print_error("[mini$Hell]: ", "memory allocation failed");
			data->status = 1;
		}
		return (NULL);
	}
	return (tokens);
}
