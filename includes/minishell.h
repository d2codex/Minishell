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

/* used in minishell_loop.c */
# define SHELL_PROMPT "[mini$HELL] "

/* used in exit_bultin.c */
# define ERR_PREFIX "[mini$Hell]: "
# define ERR_EXIT "exit: "
# define ERR_NUMERIC_ARG ": numeric argument required"
# define ERR_TOO_MANY_ARGS "too many arguments"

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

/* for builtin functions array, stores cmd and function's pointer*/
typedef struct s_builtin
{
	char	*cmd;
	int		(*f)(char **tokens, t_shell *data);
}	t_builtin;

/* =========================== */
/*           ENUMS             */
/* =========================== */

/* enum to track the quote current state - used inside the tokenizer */
typedef enum e_quote
{
	STATE_NOT_IN_QUOTE,
	STATE_IN_SINGLE_QUOTE,
	STATE_IN_DOUBLE_QUOTE
}	t_quote;

/* enum to track token state at the end of the tokenizer step */
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
int		builtin_pwd(char **tokens, t_shell *data);

/* exit.c */
int		builtin_exit(char **tokens, t_shell *data);

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

/* execute_tokenizer */
char	**execute_tokenizer(char *line, t_shell *data);
bool	validate_tokens(char **tokens, char *line);

/* execute_builtins.c */
int		execute_builtin(char **tokens, t_shell *data);

/* init_shell.c */
int		init_shell(t_shell *data, char **envp);

/* print_ascii_art.c */
void	print_ascii_art(void);

/* =========================== */
/*           PARSER            */
/* =========================== */

/* src/parser/tokenizer_utils.c */
t_quote	update_quote_state(t_quote current_quote_state, char c);
bool	is_a_shell_separator(t_quote current_quote_state, char c);
bool	has_unclosed_quotes(char const *s);
void	skip_whitespace(char const *s, size_t *i);

/* src/parser/tokenizer_count_tokens.c */
int		count_shell_tokens(const char *s);

/* src/parser/tokenizer_smart_split.c */
char	**ft_split_tokens(char const *s, t_token_error *error_code);

/* =========================== */
/*         EXECUTION           */
/* =========================== */
/* TODO: add execution module functions when implemented */

/* =========================== */
/*           UTILS             */
/* =========================== */

/* src/utils/is_whitespace.c */
bool	is_whitespace(char c);

/* src/utils/memory_cleanup.c */
void	free_string_array(char **tab, size_t count);
void	cleanup_shell(t_shell *data);
void	cleanup_process_line(char **tokens, char *line);

/* src/utils/print_errors_multi.c */
void	print_error_multi(char *part1, char *part2, char *part3, char *part4);

/* src/utils/print_error.c */
void	print_error(char *pre_msg, char *main_msg);

#endif
