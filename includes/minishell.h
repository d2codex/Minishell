#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>

// structs
typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;

int			builtin_pwd(char **args);

/* import environment */
/* src/env/env.c      */
void		del_env(void *content);
void		print_env_list(t_list *env_list);
t_env		*create_env_node(const char *str);
t_list		*init_env_from_envp(char **envp);

#endif
