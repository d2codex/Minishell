#include "minishell.h"
// delete when included in libft
int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

/**
 * @brief Execute builtin commands.
 *
 * Placeholder function trying to demonstrate the new error code propagation
 * system.
 * NOTE: I added a simple look up function, but that will change when we take
 * into account if is_child.  We can test any builtins that are in the parent.
 *
 * @param tokens Tokens array from the command line
 * @param data Shell data structure
 * @return -1 for exit signal, 0 for continue
 */
int	execute_builtin(char **tokens, t_shell *data)
{
	int						i;
	int						result;
	static const t_builtin	builtins[] =
	{
		{"pwd", builtin_pwd},
			// {"exit", builtin_exit},
			// other builtins
		{NULL, NULL}
	};

	i = 0;
	while (builtins[i].cmd != NULL)
	{
		if (ft_strcmp(tokens[0], builtins[i].cmd) == 0)
		{
			result = builtins[i].f(tokens, data);
			if (ft_strcmp(tokens[0], "exit") == 0)
				return (result);
			data->status = result;
			return (0);
		}
		i++;
	}
	return (0);
}
