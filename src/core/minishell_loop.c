#include "minishell.h"

/**
 * @brief Main interactive loop of minishell.
 *
 * Continuously prompts user and processes commands until exit or EOF.
 * Propagates final exit code to main.
 *
 * @param data Shell configuration and state
 * @return Final exit code for the shell
 */
int	minishell_loop(t_shell *data)
{
	while (1)
	{
		// better way of managing the error codes, the old version was not returning anything
		if (!prompt_user(BR_CYN SHELL_PROMPT RESET, data))
			return (data->status);
	}
}

/**
 * @brief Show prompt, read input and process it.
 *
 * @param prompt Prompt string to display
 * @param data Shell data structure
 * @return true to continue, false to break the loop on EOF or exit signal
 */
bool	prompt_user(char *prompt, t_shell *data)
{
	char	*line;
	int		result;

	if (!data->is_tty)
		return (false);
	line = readline(prompt);
	if (!line)
	{
		printf("exit\n");
		return (false);
	}
	if (line[0] == '\0')
	{
		free(line);
		return (true);
	}
	result = process_line(line, data);
	// check for exit signal -1 returned from the builtins
	if (result == -1)
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
static char	**execute_tokenizer(char *line, t_shell *data)
{
	t_token_error	error_code;
	char			**tokens;

	tokens = ft_split_tokens(line, &error_code);
	if (error_code != TOKEN_OK)
	{
		if (error_code == TOKEN_UNCLOSED_QUOTE)
		{
			print_error("mini$Hell: ", "syntax error: unclosed quotes");
			data->status = 2;
		}
		else if (error_code == TOKEN_MALLOC_ERROR)
		{
			print_error("mini$Hell: ", "memory allocation failed");
			data->status = 1;
		}
		return (NULL);
	}
	return (tokens);
}

/**
 * @brief Process a command line through the shell pipeline.
 *
 * Tokenizes input, executes builtins, and handles error propagation.
 * Will be extended for full parsing and external command execution.
 *
 * @param line User input command line
 * @param data Shell data structure
 * @return -1 for exit signal, 0 for continue, >0 for error codes
 */
int	process_line(char *line, t_shell *data)
{
	char	**tokens;
	int		result;
	int		token_count;

	// I think we can get rid of our lovely check added yesterday if we are short on lines
	if (line)
		add_history(line);
	tokens = execute_tokenizer(line, data);
	// post check after execute_tokenizer could be shortened of put in a function
	if (!tokens)
	{
		free(line);
		return (0);
	}
	if (!tokens[0])
	{
		free_string_array(tokens, 0);
		free(line);
		return (0);
	}
	// TODO: Parsing phase (AST generation, syntax validation)
	// TODO: Variable expansion ($VAR, $?, etc.)
	// TODO: Quote removal
	// TODO: Redirection setup (<, >, <<, >>)
	// TODO: Pipe setup and process management
	// Execute command
	result = execute_builtin(tokens, data);
	// TODO: External commands if not builtin...
	// cleanup - should be a dedicated function I think
	token_count = 0;
	while (tokens[token_count])
		token_count++;
	free_string_array(tokens, token_count);
	free(line);
	return (result);
}
