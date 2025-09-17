/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <diade-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 21:52:15 by diade-so          #+#    #+#             */
/*   Updated: 2024/12/11 15:56:54 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Writes a string followed by a newline to the given file descriptor.
 *
 * @param s The string to write. If NULL, nothing is printed before the newline.
 * @param fd The file descriptor (e.g., STDOUT_FILENO).
 * @return int 0 on success, -1 on write error.
 *
 * @details
 * Relies on ft_putstr_fd() and ft_putchar_fd(). Returns 0 if s is NULL
 * because printing nothing is not an error, which keeps error handling simple.
 */
int	ft_putendl_fd(char *s, int fd)
{
	if (ft_putstr_fd(s, fd) == -1)
		return (-1);
	if (ft_putchar_fd('\n', fd) == -1)
		return (-1);
	return (0);
}
