#include "minishell.h"

/* 
	NOTE: 
	- !line (ctrld+D) on bash prints "exit" and leaves bash
*/

void	process_line(char *line)
{
	add_history(line);
	printf("%s\n", line);
	free(line);
}

int	prompt_user(char *prompt)
{
	char	*line;

	line = readline(prompt);
	if (!line)
	{
		printf("exit\n");
		return (0);
	}
	if (line[0] == '\0')
	{
		free(line);
		return (1);
	}
	process_line(line);
	return (1);
}
