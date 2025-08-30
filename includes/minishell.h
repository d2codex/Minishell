#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

/* src/builtins/pwd.c */
int		builtin_pwd(char **args);

/* src/minishell_loop.c */
void	process_line(char *line);
int		prompt_user(char *prompt);

#endif
