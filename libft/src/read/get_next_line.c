/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <diade-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:56:26 by diade-so          #+#    #+#             */
/*   Updated: 2025/06/03 22:28:49 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_next_line.h"

static char	*strjoin_and_free(char *buffer, char *read_buffer)
{
	char	*joined_str;

	joined_str = ft_strjoin(buffer, read_buffer);
	free(buffer);
	buffer = NULL;
	return (joined_str);
}

static char	*extract_line(char *buffer)
{
	char	*line;
	int		i;

	i = 0;
	if (buffer[0] == '\0')
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	line = ft_calloc(i + 1, sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		line[i] = '\n';
	return (line);
}

static char	*leftover(char *buffer)
{
	char	*leftover;
	int		i;
	int		j;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	if (buffer[i] == '\0')
		return (free(buffer), buffer = NULL, NULL);
	j = 0;
	while (buffer[i + j] != '\0')
		j++;
	leftover = ft_calloc((j + 1), sizeof (char));
	if (!leftover)
		return (NULL);
	while (j--)
		leftover[j] = buffer[i + j];
	return (free(buffer), buffer = NULL, leftover);
}

static char	*read_from_file(int fd, char *buffer)
{
	char	*read_buffer;
	ssize_t	bytes_read;

	read_buffer = ft_calloc(BUFFER_SIZE + 1, sizeof (char));
	if (!read_buffer)
		return (free(buffer), buffer = NULL, NULL);
	bytes_read = 1;
	while (bytes_read > 0 && ft_strchr(buffer, '\n') == NULL)
	{
		bytes_read = read(fd, read_buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free(read_buffer), free(buffer),
				read_buffer = NULL, buffer = NULL, NULL);
		read_buffer[bytes_read] = '\0';
		buffer = strjoin_and_free(buffer, read_buffer);
		if (!buffer)
			return (free(read_buffer), read_buffer = NULL, NULL);
	}
	return (free(read_buffer), read_buffer = NULL, buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer[OPEN_MAX];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= OPEN_MAX)
		return (NULL);
	if (!buffer[fd])
		buffer[fd] = ft_calloc(1, sizeof(char));
	if (!buffer[fd])
		return (NULL);
	buffer[fd] = read_from_file(fd, buffer[fd]);
	if (!buffer[fd])
		return (NULL);
	line = extract_line(buffer[fd]);
	buffer[fd] = leftover(buffer[fd]);
	if (!buffer[fd])
		free(buffer[fd]);
	return (line);
}
