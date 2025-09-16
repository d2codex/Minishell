#include "minishell.h"

/**
* @brief Entry point of minishell.
*
* Initializes environment, shows ASCII art, starts shell loop, clear history
* and frees env resources before exiting.
*
* @param argc Argument count (unused for now)
* @param argv Argument values (unused for now)
* @param envp System environment variables
* @return Exit status (0 on success, 1 error)
*
* Compile:	make build TEST=integration/test_core_shell_loop.c
* Run:		./bin/test_core_shell_loop
* valgrind (ignore leaks from readline library):
*			make valgrind TEST=integration/test_core_shell_loop.c
*
* @note we need to think about the error codes propagation system and how the
* user will be able to print them with $? (they will be compared to actual bash
* codes during correction)
*/
int	main(int argc, char **argv, char **envp)
{
	t_list	*env_list;

	// we can delete the (void) when we actually need argc/argv
	(void)argc;
	(void)argv;
	env_list = init_env_from_envp(envp);

	// print error message and set up error/exit code if !env ?
	if (!env_list)
		return (1);

	// my precious
	print_ascii_art();
	minishell_loop(env_list);
	ft_lstclear(&env_list, del_env);
	rl_clear_history();
	return (0);
}
