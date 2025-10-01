#include "minishell.h"

/**
 * @brief Determine the target path for the cd command.
 *
 * Evaluates the command argv to figure out where cd should navigate:
 * - No arguments: returns the value of the HOME environment variable.
 * - One argument: returns that argument as the target path.
 * - More than one argument: prints an error and returns NULL.
 *
 * This function does not modify the current working directory itself.
 *
 * @param argv Array of command argv, where argv[0] is "cd".
 * @param data Shell data structure containing environment list.
 * @return Pointer to target path string, or NULL on error.
 */
static char	*get_cd_target(char **argv, t_shell *data)
{
	t_env	*home_node;

	if (argv[1] == NULL)
	{
		home_node = get_env_node_by_key(data->env_list, "HOME");
		if (!home_node || !home_node->value)
		{
			print_error(ERR_PREFIX, ERR_CD, "HOME not set", NULL);
			return (NULL);
		}
		return (home_node->value);
	}
	else if (argv[2] != NULL)
	{
		print_error(ERR_PREFIX, ERR_CD, ERR_TOO_MANY_ARGS, NULL);
		return (NULL);
	}
	return (argv[1]);
}

/**
 * @brief Updates OLDPWD in the environment list.
 *
 * Replaces the value of OLDPWD with the provided oldpwd.
 * If OLDPWD does not exist, nothing is done.
 *
 * @param data Shell data structure
 * @param oldpwd Previous working directory string
 */
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

/**
 * @brief Prints a formatted error message for cd failures.
 *
 * Outputs the error in the form:
 * "bash: cd: <target>: <system error message>\n"
 * to standard error. Does nothing if target is NULL.
 *
 * @param target The directory path that caused the error
 */
static void	print_cd_error(char *target)
{
	if (!target)
		return ;
	ft_putstr_fd("[mini$HELL]: cd: ", STDERR_FILENO);
	ft_putstr_fd(target, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
}

/**
 * @brief Set the shell status and return it.
 *
 * @param data Shell data structure
 * @param status Status code to set
 * @return The same status code
 */
static int	set_status(t_shell *data, int status)
{
	data->status = status;
	return (status);
}

/**
 * @brief Implements the cd builtin command.
 *
 * Changes the current working directory.
 * - No arguments: changes to the directory specified by $HOME.
 * - One argument: changes to the specified directory.
 * - More than one argument: prints an error and fails.
 * Updates OLDPWD in the environment if applicable.
 * Updates the shell status in data->status before returning.
 *
 * @param argv Command argv array where argv[0] is "cd"
 * @param data Shell data structure, including environment and status
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int	builtin_cd(char **argv, t_shell *data)
{
	char	*target;
	char	*oldpwd;

	if (!argv || !data)
		return (set_status(data, EXIT_FAILURE));
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("[mini$HELL: cd: ");
		return (set_status(data, EXIT_FAILURE));
	}
	target = get_cd_target(argv, data);
	if (!target)
	{
		free(oldpwd);
		return (set_status(data, EXIT_FAILURE));
	}
	if (chdir(target) != 0)
	{
		print_cd_error(target);
		free(oldpwd);
		return (set_status (data, EXIT_FAILURE));
	}
	update_old_pwd(data, oldpwd);
	return (set_status(data, EXIT_SUCCESS));
}
