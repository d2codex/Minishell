#ifndef MINISHELL_H
# define MINISHELL_H

/* =========================== */
/*          INCLUDES           */
/* =========================== */
# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

/* =========================== */
/*         CONSTANTS           */
/* =========================== */

# define SHELL_PROMPT "[mini$HELL] "

/* =========================== */
/*        STRUCTURES           */
/* =========================== */

/* env import structure to emulate KEY=VALUE behavior */
typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;

/* shell state and configuration */
typedef struct s_shell
{
	t_list	*env_list;
	int		status;
	bool	is_tty;
	bool	is_child;
}	t_shell;

/* =========================== */
/*           ENUMS             */
/* =========================== */

/* enum to track the quote current state - used inside the tokeniser */
typedef enum e_quote
{
	STATE_NOT_IN_QUOTE,
	STATE_IN_SINGLE_QUOTE,
	STATE_IN_DOUBLE_QUOTE
}	t_quote;

/* enum to track token state at the end of the tokeniser step */
typedef enum e_token_error
{
	TOKEN_OK,
	TOKEN_MALLOC_ERROR,
	TOKEN_UNCLOSED_QUOTE,
	TOKEN_NOT_OPERATOR
}	t_token_error;

/* =========================== */
/*          BUILTINS           */
/* =========================== */

/* pwd.c */
int		builtin_pwd(char **args, t_shell *data);

/* execute_builtins.c */
int		execute_builtin(char **tokens, t_shell *data);

/* =========================== */
/*     ENVIRONMENT IMPORT      */
/* =========================== */

/* src/env/env.c      */
void	del_env(void *content);
void	print_env_list(t_list *env_list);
t_env	*create_env_node(const char *str);
t_list	*init_env_from_envp(char **envp);

/* =========================== */
/*            CORE             */
/* =========================== */

/* minishell_loop.c */
int		minishell_loop(t_shell *data);
bool	prompt_user(char *prompt, t_shell *data);
int		process_line(char *line, t_shell *data);

/* print_ascii_art.c */
void	print_ascii_art(void);

/* =========================== */
/*           PARSER            */
/* =========================== */

/* src/parser/tokeniser_utils.c */
t_quote	update_quote_state(t_quote current_quote_state, char c);
bool	is_a_shell_separator(t_quote current_quote_state, char c);
bool	has_unclosed_quotes(char const *s);
void	skip_whitespace(char const *s, size_t *i);

/* src/parser/tokeniser_count_tokens.c */
int		count_shell_tokens(const char *s);

/* src/parser/tokeniser_smart_split.c */
char	**ft_split_tokens(char const *s, t_token_error *error_code);

/* =========================== */
/*         EXECUTION           */
/* =========================== */
/* TODO: add execution module functions when implemented */

/* =========================== */
/*           UTILS             */
/* =========================== */

/* src/utils/free_strings_array.c */
void	free_string_array(char **tab, size_t count);

/* src/utils/is_whitespace.c */
bool	is_whitespace(char c);

/* src/utils/print_error.c */
void	print_error(char *pre_msg, char *main_msg);

#endif
