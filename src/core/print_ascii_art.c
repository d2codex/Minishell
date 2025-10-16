/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ascii_art.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:40:33 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/15 17:40:34 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief check if input is the binary easter egg sequence
 *
 * @param line input line from user
 * @return true if it's the easter egg, false otherwise
 */
bool	is_easter_egg(char *line)
{
	if (!line)
		return (false);
	return (ft_strcmp(line, EASTER_EGG) == 0);
}

/**
 * @brief display easter egg message
 */
void	display_easter_egg(void)
{
	printf(BR_RED " ! SPECIAL SEQUENCE DETECTED ! \n" RESET);
	printf(BR_GRN "   decoding: 101010 = 42\n" RESET);
	printf(BR_YEL "   ┌─────────────────────────────┐\n" RESET);
	printf(BR_YEL "   │ ! you've found the answer ! │\n" RESET);
	printf(BR_YEL "   │    ! outstanding move !     │\n" RESET);
	printf(BR_YEL "   └─────────────────────────────┘\n" RESET);
	printf(RED "back to work now...\n" RESET);
}

/**
 * @brief Check and handle easter egg if detected.
 *
 * Checks if the input line is the easter egg sequence. If so, displays
 * the easter egg message, frees the line, and returns true to signal
 * that processing is complete.
 *
 * @param line Input line from user
 * @return true if easter egg was detected and handled, false otherwise
 */
bool	check_and_handle_easter_egg(char *line)
{
	if (is_easter_egg(line))
	{
		display_easter_egg();
		free(line);
		return (true);
	}
	return (false);
}

/**
 * @brief Selects and displays a random ASCII art
 *
 * Not a lot of ways to achieve randomness with the list of our allowed
 * functions. The trick is a pseudo random number generator based on memory
 * addresses and file descriptors to select one of 5 ASCII art variants.
 * The randomness is derived from:
 * - Stack variable address (&dummy)
 * - Heap allocation address (malloc)
 * - File descriptor value (dup)
 *
 * @note This function performs memory allocation and file operations
 * solely for randomness generation, not for functional purposes.
 * Everything is freed before continuing the flow.
 */
void	select_random_ascii_art(void)
{
	int		dummy;
	void	*ptr;
	int		fd;
	int		choice;

	ptr = malloc(1);
	if (!ptr)
		return (print_ascii_art_hell_simple());
	fd = dup(0);
	choice = (((unsigned long)&dummy) + ((unsigned long)ptr) + fd) % 5;
	if (ptr)
		free(ptr);
	if (fd != -1)
		close(fd);
	if (choice == 0)
		print_ascii_art_hell_simple();
	else if (choice == 1)
		print_ascii_art_green_breach();
	else if (choice == 2)
		print_ascii_art_fire_colors();
	else if (choice == 3)
		print_ascii_art_easter_egg();
	else
		print_ascii_art_dark();
}
