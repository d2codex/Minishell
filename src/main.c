/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 18:07:58 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/15 18:07:59 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	init_shell(&data, envp);
	setup_signals_interactive();
	if (data.is_tty)
		select_random_ascii_art();
	data.status = minishell_loop(&data);
	cleanup_shell(&data);
	return (data.status);
}
