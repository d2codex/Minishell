#include "minishell.h"

/**
* @brief Entry point of [minis$Hell].
*
* Initializes shell data structure, shows ASCII art, starts shell loop,
* cleans up resources and exits with proper code.
*
* @param argc Argument count (unused for now)
* @param argv Argument values (unused for now)
* @param envp System environment variables
* @return Exit status from shell execution
*/
int	main(int argc, char **argv, char **envp)
{
	t_shell	data;

	(void)argc;
	(void)argv;
	if (init_shell(&data, envp))
		return (1);
	if (data.is_tty)
		print_random_ascii_art();
	data.status = minishell_loop(&data);
	cleanup_shell(&data);
	return (data.status);
}
