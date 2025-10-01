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
	// process the command line (builtin or external)
	data->status = process_line(line, data);
	if (data->should_exit)
		return (false);
	return (true);
}

/**
 * @brief Tokenize a command line and create a token type list.
 *
 * This function takes a raw command line and:
 *  1. Tokenizes it into an array of strings via `execute_tokenizer`.
 *  2. Validates the tokens with `validate_tokens`.
 *  3. Builds a linked list of typed tokens via `create_token_type_list`.
 *
 * If tokenization or validation fails, the function returns EXIT_FAILURE.
 *
 * @param line The raw input command line string.
 * @param data Shell state structure, used for tokenizer context.
 * @param tokens Pointer to store the resulting array of token strings.
 * @param token_list Pointer to store the resulting typed token list.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE if tokenization or
 * validation fails.
 */
static int	process_tokens(char *line, t_shell *data,
	char ***tokens, t_token **token_list)
{
	*tokens = execute_tokenizer(line, data);
	if (!validate_tokens(*tokens))
		return (EXIT_FAILURE); // invalid tokens = syntax misuse
	*token_list = create_token_type_list(*tokens);
	if (!*token_list)
		return (EXIT_FAILURE); // malloc or internal error
	return (EXIT_SUCCESS);
}

/**
 * @brief Build an AST from a token list and assign node details.
 *
 * This function takes a linked list of typed tokens and performs the following
 * steps:
 *  1. Creates a linear AST list using `create_ast_list`.
 *  2. Assigns node types (CMD, PIPE, REDIR) via `assign_ast_node_type`.
 *  3. Validates AST syntax with `validate_syntax_ast_list`.
 *  4. Collects argv arrays and filenames for CMD and REDIR nodes via
 * `assign_argv_and_filename`.
 *
 * If any step fails, the function returns an appropriate error code:
 *  - EXIT_FAILURE for allocation or internal errors.
 *  - MISUSAGE_ERROR for syntax errors.
 *
 * @param token_list The input typed token list.
 * @param ast Pointer to store the resulting AST list.
 * @return EXIT_SUCCESS on success, otherwise an error code indicating
 * the failure.
 */
static int	process_ast(t_token *token_list, t_ast **ast, t_shell *data)
{
	int	status;

	*ast = create_ast_list(token_list);
	if (!*ast)
		return (EXIT_FAILURE);
	assign_ast_node_type(*ast);
	status = validate_syntax_ast_list(*ast);
	if (status != EXIT_SUCCESS)
		return (status); // already returns MISUSAGE_ERROR for bad syntax
	expand_ast_nodes(*ast, data);
	trim_quotes_in_ast(*ast);
	status = assign_argv_and_filename(*ast);
	if (status != EXIT_SUCCESS)
		return (status); // could return EXIT_FAILURE or something custom
	return (EXIT_SUCCESS);
}

/**
 * @brief Process a single input line in the shell.
 *
 * This function performs the full processing of a command line:
 *  1. Handles Easter egg commands via `is_easter_egg` and `display_easter_egg`.
 *  2. Adds non-empty lines to the history if in TTY mode.
 *  3. Tokenizes and validates the line via `process_tokens`.
 *  4. Builds and validates the AST via `process_ast`.
 *  5. Executes the command as a builtin or external program.
 *  6. Cleans up all allocated resources.
 *
 * The function returns the exit status of the command:
 *  - EXIT_SUCCESS (0) for successful execution or Easter eggs.
 *  - MISUSAGE_ERROR for syntax errors detected in tokens or AST.
 *  - EXIT_FAILURE for allocation or internal errors.
 *  - The actual exit code of builtins or external commands otherwise.
 *
 * @param line The input command line to process.
 * @param data Shell context storing state, status, and execution info.
 * @return The resulting exit code of the processed line.
 */
int	process_line(char *line, t_shell *data)
{
	char	**tokens;
	t_ast	*ast_list;
	t_token	*token_list;
	int		result;

	tokens = NULL;
	ast_list = NULL;
	token_list = NULL;
	if (is_easter_egg(line))
	{
		display_easter_egg();
		free(line);
		return (EXIT_SUCCESS);
	}
	if (line)
		add_history(line);
	result = process_tokens(line, data, &tokens, &token_list);
	if (result != EXIT_SUCCESS)
		return(cleanup_process_line(tokens, NULL, token_list, line), result);
	result = process_ast(token_list, &ast_list, data);
	if (result != EXIT_SUCCESS)
		return (cleanup_process_line(tokens, ast_list, token_list, line), result);
	//sync_tokens_with_ast(tokens, ast_list);
	if (!execute_builtin(ast_list, data))
		result = execute_external_command(tokens, data);
	else
		result = data->status;
	cleanup_process_line(tokens, ast_list, token_list, line);
	return (result);
}
