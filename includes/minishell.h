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

/* =========================== */
/*           ENUMS             */
/* =========================== */

/* enum to track the quote current state - used inside the tokeniser */
typedef enum e_quote
{
	STATE_NORMAL,
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
int		builtin_pwd(char **args);

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
void	minishell_loop(t_list *env_list);
bool	prompt_user(char *prompt, t_list *env_list);
void	process_line(char *line, t_list *env_list);

/* print_ascii_art.c */
void	print_ascii_art(void);

/* =========================== */
/*           PARSER            */
/* =========================== */

/* src/parser/tokeniser_utils.c */
t_quote	update_quote_state(t_quote current_quote_state, char c);
bool	is_a_shell_separator(t_quote current_quote_state, char c);
bool	has_unclosed_quotes(char const *s);

/* src/parser/tokeniser_count_tokens.c */
int		count_shell_tokens(char const *s);
void	skip_whitespace(char const *s, size_t *i);

/* src/parser/tokeniser_smart_split.c */
char	**ft_split_tokens(char const *s, t_token_error *error_code);

/* =========================== */
/*         EXECUTION           */
/* =========================== */
/* TODO: Add execution module functions when implemented */

/* =========================== */
/*           UTILS             */
/* =========================== */

void	free_string_array(char **tab, size_t count);
bool	is_whitespace(char c);
void	print_error(char *pre_msg, char *main_msg);

#endif
