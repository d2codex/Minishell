/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <diade-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 20:22:33 by diade-so          #+#    #+#             */
/*   Updated: 2024/12/09 12:20:54 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

/**
 * @brief Writes a single character to the given file descriptor.
 *
 * @param c The character to write.
 * @param fd The file descriptor (e.g., STDOUT_FILENO).
 * @return int 0 on success, -1 on write error.
 *
 * @details
 * Uses `ssize_t` to capture write() return value, which is -1 on error.
 * Returns 0 on success to simplify error checking in calling functions.
 */
int	ft_putchar_fd(char c, int fd)
{
	ssize_t	ret;

	ret = write(fd, &c, 1);
	if (ret == -1)
		return (-1);
	return (0);
}
