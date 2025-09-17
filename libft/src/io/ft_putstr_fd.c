/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <diade-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 21:42:32 by diade-so          #+#    #+#             */
/*   Updated: 2024/12/11 15:50:06 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

/**
 * @brief Writes a null-terminated string to the given file descriptor.
 *
 * @param s The string to write. If NULL, nothing is printed.
 * @param fd The file descriptor (e.g., STDOUT_FILENO).
 * @return int 0 on success or if s is NULL, -1 on write error.
 *
 * @details
 * - Uses `ssize_t` to capture the return value of write().
 * - Returns 0 if s is NULL because printing "nothing" is not
 *   considered an error.
 */
int	ft_putstr_fd(char *s, int fd)
{
	ssize_t	ret;

	if (!s)
		return (0);
	ret = write (fd, s, ft_strlen(s));
	if (ret == -1)
		return (-1);
	return (0);
}
