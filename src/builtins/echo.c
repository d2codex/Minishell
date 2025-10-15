/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:39:22 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/15 17:39:23 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if arg is a valid bash style -n option for echo.
 *
 * Accepts "-n", "-nn", "-nnn", etc. but rejects "-na", "-n1", etc.
 * Implements bash's quirky but exact -n recognition behavior.
 *
 * @param arg String to check
 * @return true if valid -n option, false otherwise
 */
static bool	is_echo_n_option(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (false);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

/**
 * @brief Print argv starting at given index, space separated.
 *
 * Prints each arg with single spaces between them (no space after last).
 * Helper function for echo output formatting.
 *
 * @param argv NULL-terminated array of strings
 * @param start_index Index to start printing from
 */
static void	print_echo_cmd(char **argv, int start_print_index)
{
	while (argv[start_print_index] != NULL)
	{
		printf("%s", argv[start_print_index]);
		if (argv[start_print_index + 1])
			printf(" ");
		start_print_index++;
	}
}

/**
 * @brief Implements the echo builtin command.
 *
 * Prints the given arguments to standard output, separated by spaces.
 * Supports the -n option to suppress the trailing newline. Multiple
 * consecutive -n options are all consumed and treated as a single option.
 *
 * Behavior:
 * - echo (no args): prints a newline
 * - echo args: prints args separated by spaces, followed by newline
 * - echo -n: prints nothing (no newline)
 * - echo -n args: prints args separated by spaces, no trailing newline
 * - echo -n -n -n args: same as echo -n args (multiple -n consumed)
 * - echo -n -nnnnnnnnnnnn: chained options, works like -n
 *
 * The -n option is only effective when it appears consecutively at the
 * beginning of the argument list.
 *
 * @param argv Command argv array where argv[0] is "echo"
 * @param data Shell data structure (unused in this builtin)
 * @return Always returns EXIT_SUCCESS (0), updates `data->status`.
 */
int	builtin_echo(char **argv, t_shell *data)
{
	int		start_print_index;
	bool	print_new_line;

	if (argv[1] == NULL)
	{
		printf("\n");
		data->status = EXIT_SUCCESS;
		return (data->status);
	}
	start_print_index = 1;
	print_new_line = true;
	while (argv[start_print_index]
		&& is_echo_n_option(argv[start_print_index]))
	{
		print_new_line = false;
		start_print_index++;
	}
	print_echo_cmd(argv, start_print_index);
	if (print_new_line)
		printf("\n");
	data->status = EXIT_SUCCESS;
	return (data->status);
}
