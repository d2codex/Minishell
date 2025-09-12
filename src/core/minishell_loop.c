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
		if (data->is_tty)
		{
			if (!prompt_user(BR_CYN SHELL_PROMPT RESET, data))
				return (data->status);
		}
		else
		{
			if (!prompt_user(NULL, data))
				return (data->status);
		}
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

	if (data->is_tty)
		line = readline(prompt);
	else
		line = readline(NULL);
	if (!line)
	{
		if (data->is_tty)
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
 * @brief Process a command line through the shell pipeline.
 *
 * Clean interface function that orchestrates the full command processing:
 * tokenization → validation → execution → cleanup
 *
 * @param line User input command line
 * @param data Shell data structure
 * @return -1 for exit signal, 0 for continue, >0 for error codes
 */
int	process_line(char *line, t_shell *data)
{
	char	**tokens;
	int		result;

	if (line)
		add_history(line);
	tokens = execute_tokenizer(line, data);
	// validate tokens (handles cleanup on error)
	if (!validate_tokens(tokens, line))
		return (0);

	// TODO: Parsing phase (AST generation, syntax validation)
	// TODO: Variable expansion ($VAR, $?, etc.)
	// TODO: Quote removal
	// TODO: Redirection setup (<, >, <<, >>)
	// TODO: Pipe setup and process management

	// Execute command
	result = execute_builtin(tokens, data);
	// TODO: External commands if not builtin...

	// clean up resources
	cleanup_process_line(tokens, line);

	return (result);
}
