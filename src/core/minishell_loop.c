#include "minishell.h"

/**
* @brief Main interactive loop of minishell.
*
* Repeatedly calls prompt_user() until EOF (Ctrl+D) or exit command.
* Exit command will be handled later in the execution phase.
*
* @param env_list Environment variables list
*/
void	minishell_loop(t_list *env_list)
{
	int	running;

	running = 1;
	while (running)
		running = prompt_user(BR_CYN SHELL_PROMPT RESET, env_list);
}

/**
* @brief Show the prompt, read input and process it.
*
* Handles EOF (Ctrl+D → exit) and ignores empty lines.
* For now, signals (Ctrl+C) are left to readline, not fully subject
* compliant yet. Signal handling will be added later.
*
* @param prompt Prompt string to display
* @param env_list Environment variables
* @return true to continue shell loop, false to exit
*/
bool	prompt_user(char *prompt, t_list *env_list)
{
	char	*line;

	line = readline(prompt);
	// EOF (Ctrl+D) detected on empty line
	// Note: Ctrl+D on non-empty line is ignored by readline (bash behavior)
	if (!line)
	{
		printf("exit\n");
		return (false);
	}
	// special case: user just hits ENTER
	// special treatment: we don't want to waste time calling process_line()
	// the shell should still run (true) and prompts the user again
	if (line[0] == '\0')
	{
		free(line);
		return (true);
	}
	process_line(line, env_list);
	return (true);
}

/**
* @brief Process a command line through the shell pipeline.
*
* Planned pipeline:
* 1) Tokenize 2) Parse 3) Execute 4) Cleanup
* For now for the basic loop, just printf the input to
* demonstrate how the shell loop work without the parsing, exe, cleanup part.
*
* @param line User input
* @param env_list Environment variables
* @todo Add tokenizer, parser, execution and error handling
*/
void	process_line(char *line, t_list *env_list)
{
	// not used yet, delete this line when env is used
	(void)env_list;
	add_history(line);
	/*
	 * architecture proposal - 1000% open to discussion !
	 *
	 * 1. TOKENIZATION PHASE:
	 *	tokens = ft_split_tokens(line, &error_code);
	 *	if (error_code != TOKEN_OK)
	 *	{
	 *		- function to handle printing error message and assigning right exit
	 *		codes, unclosed quotes, malloc errors...
	 *		- function for cleanup and return/exit
	 *	}
	 *
	 * 2. PARSING PHASE:
	 *	commands = parse_tokens(tokens); -- handle pipes, redirections...
	 *	if (!commands)
	 *	{
	 *		- function to handle printing error message and assigning right exit
	 *		codes, malloc errors, etc.
	 *		- function for cleanup and return/exit
	 *	}
	 *
	 * 3. EXECUTION PHASE:
	 *	execute_commands(commands, env_list);
	 *
	 *	Where execute_commands() will:
	 *	- Check if command is builtin (pwd, cd, env, ...)
	 *	- if builtin: call execute_builtin(command, env_list)
	 *	- if external: call execute_external(command, env_list) - fork + execve
	 *
	 * 4. CLEANUP PHASE:
	 *	- free tokens related resources
	 *	- free command related resources
	 */
	// because we have nothing integrated here yet, just a little printf so that
	// the shell does something while waiting to be a real grown up shell
	printf("%s\n", line);
	free(line);
}
