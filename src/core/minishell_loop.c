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
 * via `process_line`. Updates `data->status` with the exit code of the
 * last command. Handles empty input and end-of-file (Ctrl+D).
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
	data->status = process_line(line, data);
	if (data->should_exit)
		return (false);
	return (true);
}

/**
 * @brief Tokenize and prepare a validated token list from a command line.
 *
 * This function performs all stages of token processing:
 *  1. Splits the raw input line into string tokens using `execute_tokenizer`.
 *  2. Validates basic token structure via `validate_tokens`.
 *  3. Builds a typed linked list with `create_token_type_list`.
 *  4. Validates token syntax using `validate_syntax_token_list`.
 *  5. Expands variables and wildcards through `expand_tokens_list`.
 *  6. Removes surrounding quotes via `trim_quotes_in_token_list`.
 *
 * If any stage fails, allocated memory is freed and an error code is returned.
 *
 * @param line        The raw input command line string.
 * @param data        The shell context containing environment and state.
 * @param tokens      Output pointer to store the resulting string token array.
 * @param token_list  Output pointer to store the resulting typed token list.
 * @return EXIT_SUCCESS on success,
 *         MISUSAGE_ERROR on syntax errors,
 *         or EXIT_FAILURE on memory or internal errors.
 */
static int	process_tokens(char *line, t_shell *data,
	char ***tokens, t_token **token_list)
{
	*tokens = execute_tokenizer(line, data);
	if (!validate_tokens(*tokens))
		return (EXIT_FAILURE);
	*token_list = create_token_type_list(*tokens);
	if (!*token_list)
		return (EXIT_FAILURE);
	if (validate_syntax_token_list(*token_list) != EXIT_SUCCESS)
		return (MISUSAGE_ERROR);
	if (expand_tokens_list(*token_list, data) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (trim_quotes_in_token_list(*token_list) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/**
 * @brief Process a single input line in the shell.
 *
 * This function performs the full lifecycle of processing a command line:
 *  1. Detects and displays Easter egg commands using `is_easter_egg` and `display_easter_egg`.
 *  2. Adds non-empty lines to the command history (when running interactively).
 *  3. Tokenizes and validates the input via `process_tokens`.
 *  4. Builds an Abstract Syntax Tree (AST) from the token list using `build_ast_from_tokens`.
 *  5. Executes the AST via `execute_ast_tree`.
 *  6. Cleans up all allocated memory (tokens, AST, and input line).
 *
 * The function returns the resulting exit status:
 *  - EXIT_SUCCESS (0) for successful execution or Easter eggs.
 *  - Syntax or validation errors propagate from `process_tokens`.
 *  - EXIT_FAILURE for allocation or internal errors.
 *  - The actual exit code of builtins or external commands otherwise.
 *
 * @param line The input command line to process.
 * @param data The shell context storing environment, status, and execution state.
 * @return The exit code resulting from processing the given line.
 */
int	process_line(char *line, t_shell *data)
{
	char	**tokens;
	t_ast	*ast;
	t_token	*token_list;

	tokens = NULL;
	ast = NULL;
	token_list = NULL;
	if (is_easter_egg(line))
	{
		display_easter_egg();
		free(line);
		return (EXIT_SUCCESS);
	}
	if (line)
		add_history(line);
	data->status = process_tokens(line, data, &tokens, &token_list);
	if (data->status != EXIT_SUCCESS)
		return (cleanup_line(tokens, token_list, NULL, line), data->status);
	ast = build_ast_from_tokens(token_list);
	if (!ast)
		return (cleanup_line(tokens, token_list, NULL, line), EXIT_FAILURE);
//	print_ast(ast, 0);
	data->status = execute_ast_tree(ast, data);
	cleanup_line(tokens, token_list, ast, line);
	return (data->status);
}
