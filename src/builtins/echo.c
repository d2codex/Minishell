#include "minishell.h"

int	builtin_echo(char **tokens, t_shell *data)
{
	int		i;
	bool	print_new_line;

	// case - echo
	if (tokens[1] == NULL)
		printf("\n");
	else if (tokens[1] == "-n")
	{
		while (tokens[i] )
	}

	return (0);
}
