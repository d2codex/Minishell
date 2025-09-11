#include "minishell.h"

/**
* @brief Entry point of minishell.
*
* Initializes shell data structure, shows ASCII art, starts shell loop,
* cleans up resources and exits with proper code.
*
* @param argc Argument count (unused for now)
* @param argv Argument values (unused for now)
* @param envp System environment variables
* @return Exit status from shell execution
*
* Compile:	make build TEST=integration/test_core_shell_loop_refactored.c
* Run:		./bin/test_core_shell_loop
* valgrind (ignore leaks from readline library):
*			make valgrind TEST=integration/test_core_shell_loop_refactored.c
*
*/
int	main(int argc, char **argv, char **envp)
{
	t_shell	data;
	int		exit_code;

	(void)argc;
	(void)argv;
	// initialize shell data structure, I'm letting you do the init function
	data.env_list = init_env_from_envp(envp);
	data.status = 0;
	data.is_tty = isatty(STDIN_FILENO);
	data.is_child = false;
	if (!data.env_list)
		return (1);
	print_ascii_art();
	// run shell and get final exit code
	exit_code = minishell_loop(&data);
	// cleanup resources
	ft_lstclear(&data.env_list, del_env);
	rl_clear_history();
	return (exit_code);
}
