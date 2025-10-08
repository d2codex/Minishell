#include "minishell.h"

static bool	is_executable(const char *path)
{
	// TODO: Utiliser access(path, X_OK)
	// Retourner true si access() == 0
	// Retourner false sinon
}

static char	*search_in_path(const char *cmd, char **path_dirs)
{
	// TODO: Pour chaque dossier dans path_dirs
	//   - Construire le chemin avec build_path()
	//   - Tester avec is_executable()
	//   - Si trouvé, retourner le chemin
	//   - Sinon, free et continuer
	// Si rien trouvé, retourner NULL
}

char	*find_executable(char *cmd, t_shell *data)
{
	char	*path_value;
	char	**path_dirs;
	char	*result;

	if (!cmd || !cmd[0])
		return (NULL);

	// TODO 1 : Si cmd contient '/', c'est déjà un chemin
	//   - Tester avec is_executable()
	//   - Si ok, retourner ft_strdup(cmd)
	//   - Sinon, retourner NULL

	// TODO 2 : Récupérer la valeur de PATH
	//   path_value = get_var_value("PATH", data);
	//   Si NULL, retourner NULL

	// TODO 3 : Splitter PATH par ':'
	//   path_dirs = ft_split(path_value, ':');
	//   Si NULL, retourner NULL

	// TODO 4 : Chercher dans PATH
	//   result = search_in_path(cmd, path_dirs);

	// TODO 5 : Libérer path_dirs
	//   free_strings_array(path_dirs);

	return (result);
}
