#include "minishell.h"

// determine the target path
static char	*get_cd_target(char **tokens, t_shell *data)
{
	t_env	*home_node;

	if (tokens[1] == NULL) //no args, go home
	{
		// search for the HOME path in the env_list
		home_node = get_env_node_by_key(data->env_list, "HOME");
		if (!home_node || !home_node->value)
		{
			print_error(ERR_PREFIX, ERR_CD, "HOME not set", NULL);
			return (NULL);
		}
		return (home_node->value);
	}
	else if (tokens[2] != NULL) // more than one arg
	{
		print_error(ERR_PREFIX, ERR_CD, ERR_TOO_MANY_ARGS, NULL);
		return (NULL);
	}
	return (tokens[1]);
}

static void	update_old_pwd(t_shell *data, char *oldpwd)
{
	t_env	*node;

	if (!data || !oldpwd)
		return ;
	node = get_env_node_by_key(data->env_list, "OLDPWD");
	if (node)
	{
		free(node->value);
		node->value = oldpwd;
	}
}

static void	print_cd_error(char *target)
{
	if (!target)
		return ;
	ft_putstr_fd("bash: cd: ", STDERR_FILENO);
	ft_putstr_fd(target, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
}

int	builtin_cd(char **tokens, t_shell *data)
{
	char	*target;
	char	*oldpwd;

	if (!tokens || !data)
		return (EXIT_FAILURE);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("bash: cd: ");
		return (EXIT_FAILURE);
	}
	target = get_cd_target(tokens, data);
	if (!target)
		return (free(oldpwd), EXIT_FAILURE);
	if (chdir(target) != 0)
	{
		print_cd_error(target);
		free(oldpwd);
		return (EXIT_FAILURE);
	}
	update_old_pwd(data, oldpwd);
	return (EXIT_SUCCESS);
}
