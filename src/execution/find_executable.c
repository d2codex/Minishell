/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_executable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:41:13 by pafroidu          #+#    #+#             */
/*   Updated: 2025/10/15 17:41:14 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if a path points to an executable file.
 *
 * Tests whether the file at the given path exists and has
 * execute permissions using the access() system call.
 *
 * @param path File path to test
 * @return true if the file exists and is executable, false otherwise
 */
static bool	is_executable(const char *path)
{
	if (access(path, F_OK | X_OK) == 0)
		return (true);
	return (false);
}

/**
 * @brief Build a full path by joining directory and command name.
 *
 * Constructs a path string in the format "dir/cmd" by concatenating
 * the directory, a forward slash, and the command name.
 *
 * @param dir Directory path (ex: "/bin")
 * @param cmd Command name (ex: "ls")
 * @return Newly allocated full path (ex: "/bin/ls"), or NULL on malloc failure
 *
 * @note Caller is responsible for freeing the returned string.
 * @note The returned path is not tested for existence or executability.
 */
static char	*build_path(const char *dir, const char *cmd)
{
	char	*temp_path;
	char	*full_path;

	temp_path = ft_strjoin(dir, "/");
	if (!temp_path)
		return (NULL);
	full_path = ft_strjoin(temp_path, cmd);
	free(temp_path);
	if (!full_path)
		return (NULL);
	return (full_path);
}

/**
 * @brief Search for a command in the directories listed in PATH.
 *
 * Iterates through each directory in the path_dirs array, constructs
 * the full path to the command, and checks if it's executable.
 * Returns the first valid executable path found.
 *
 * @param cmd Command name to search for (ex: "ls")
 * @param path_dirs NULL-terminated array of directory paths from $PATH
 * @return Newly allocated full path to the executable, or NULL if not found
 *
 * @note Caller is responsible for freeing the returned string.
 */
static char	*search_in_path(const char *cmd, char **path_dirs)
{
	int		i;
	char	*path;

	i = 0;
	while (path_dirs[i])
	{
		path = build_path(path_dirs[i], cmd);
		if (!path)
			return (NULL);
		if (is_executable(path) == true)
			return (path);
		else
			free(path);
		i++;
	}
	return (NULL);
}

/**
 * @brief Find the full path to an executable command.
 *
 * Locates a command by searching in the directories specified by the
 * PATH environment variable. If the command contains a '/', it's treated
 * as a direct path and tested for executability without searching PATH.
 *
 * Behavior:
 * - If cmd contains '/': tests the path directly (ex: "/bin/ls", "./prog")
 * - Otherwise: searches for cmd in all directories listed in $PATH
 * - Returns NULL if the command is not found or not executable
 *
 * @param cmd Command name or path (ex: "ls" or "/bin/ls")
 * @param data Shell data structure containing environment variables
 * @return Newly allocated full path to the executable, or NULL if not found
 *
 * @note Caller is responsible for freeing the returned string.
 * @note Returns NULL if PATH is not set in the environment.
 */
char	*find_executable(char *cmd, t_shell *data)
{
	char	*path_value;
	char	**path_dirs;
	char	*result;

	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/') != NULL)
	{
		if (is_executable(cmd) == true)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_value = get_var_value("PATH", data);
	if (!path_value)
		return (NULL);
	path_dirs = ft_split(path_value, ':');
	free(path_value);
	if (!path_dirs)
		return (NULL);
	result = search_in_path(cmd, path_dirs);
	free_strings_array(path_dirs);
	return (result);
}
