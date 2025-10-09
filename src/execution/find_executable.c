#include "minishell.h"

static bool	is_executable(const char *path)
{
	if (access(path, F_OK | X_OK) == 0)
		return (true);
	return (false);
}

// full_path not freed in this function !!
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

char	*find_executable(char *cmd, t_shell *data)
{
	char	*path_value;
	char	**path_dirs;
	char	*result;

	if (!cmd || !cmd[0])
		return (NULL);
	// if cmd has a '/' already it is a path
	if (ft_strchr(cmd, '/') != NULL)
	{
		if (is_executable(cmd) == true)
			return (ft_strdup(cmd));
		return (NULL);
	}
	// normal case
	path_value = get_var_value("PATH", data);
	if (!path_value)
		return (NULL);
	path_dirs = ft_split(path_value, ':');
	if (!path_dirs)
		return (NULL);
	result = search_in_path(cmd, path_dirs);
	free_strings_array(path_dirs);
	return (result);
}
