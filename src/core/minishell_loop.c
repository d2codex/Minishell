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
 * Reads a line from stdin (with prompt if in TTY mode) and processes it 
 * via `process_line`. Handles empty input and end-of-file (Ctrl+D). 
 * Returns whether the shell should continue running or exit, based on 
 * `data->should_exit`.
 *
 * @param prompt Prompt string to display
 * @param data Shell data structure
 * @return true to continue, false to break the loop on EOF or exit signal
 */
bool	prompt_user(char *prompt, t_shell *data)
{
	char	*line;

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
	// process the command line (builtin or external)
	process_line(line, data);
	if (data->should_exit)
		return (false);
	return (true);
}

// temporary stub until execute_external_command is implemented
int	execute_external_command(char **tokens, t_shell *data)
{
	(void)tokens;
	(void)data;
	// just indicate command not found for now
	return 127; // CMD_NOT_FOUND_ERROR
}

/**
 * @brief Process a command line through the shell pipeline.
 *
 * Clean interface function that orchestrates the full command processing:
 * tokenization → validation → execution → cleanup
 * Builtins are executed first, if not a builtin, the command is run ass
 * an external command. Updates 'data->status` with the code of the last
 * command executed, including non-fatal failures. The shell continues
 * even if a command fails, but data->status reflects the most recent failure.
 *
 * @param line User input command line
 * @param data Shell data structure
 * @return Exit status of the last command executed (builtin or external).
 */
int	process_line(char *line, t_shell *data)
{
	char	**tokens;
	int		result;

	if (is_easter_egg(line))
	{
		display_easter_egg();
		free(line);
		return (EXIT_SUCCESS); // continue shell
	}
	if (line)
		add_history(line);
	tokens = execute_tokenizer(line, data);

	// if invalid, stop processing this line and report failure
	// this does not exit the whole shell, only this command cycle
	if (!validate_tokens(tokens, line))
		return (EXIT_FAILURE);
	// TODO: Parsing phase (AST generation, syntax validation)
	// TODO: Variable expansion ($VAR, $?, etc.)
	// TODO: Quote removal
	// TODO: Redirection setup (<, >, <<, >>)
	// TODO: Pipe setup and process management
	// Execute command
	
	//try builtins first (updates data->status internally)
	if (!execute_builtin(tokens, data))
	{
		// if not a builtin: execute as external cmd
		result = execute_external_command(tokens, data); //need to code this function
		data->status = result;
	}
	cleanup_process_line(tokens, line);
	// always returns the last cmd's exit status
	// we no longer need to signal to prompt_user -1 exit signal anymore
	// it is directly taken care of by the should_exit flag
	return (data->status);
}
