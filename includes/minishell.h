#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>

/* src/minishell_loop.c */
void	process_line(char *line);
int		prompt_user(char *prompt);

/* src/builtin/pwd.c */
int		builtin_pwd(char **args);

#endif 