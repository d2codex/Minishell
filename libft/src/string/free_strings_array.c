/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_strings_array.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <diade-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:58:44 by diade-so          #+#    #+#             */
/*   Updated: 2025/09/25 14:51:48 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Frees a NULL-terminated array of strings.
 *
 * This function iterates through the array until a NULL pointer is reached,
 * freeing each string and then the array itself. The size of the array does
 * not need to be known in advance.
 *
 * @param tab The NULL-terminated string array to free. Safe to pass NULL.
 */
void	free_strings_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
