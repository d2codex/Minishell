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
# define SHELL_EXIT_SIGNAL -1
# define ERR_PREFIX "[mini$Hell]: "
# define ERR_EXIT "exit: "
# define ERR_ENV "env: "
# define ERR_NUMERIC_ARG ": numeric argument required"
# define ERR_TOO_MANY_ARGS "too many arguments"

/* easter_egg.c */
# define EASTER_EGG "101010"

/* =========================== */
/*        STRUCTURES           */
/* =========================== */

/* env import structure to emulate KEY=VALUE behavior */
typedef struct s_env
{
	char	*key;
	char	*value;
	bool	in_env;
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

/* detects operation: assign, append, or key only mode */
typedef enum e_export_op
{
	EXPORT_NONE,
	EXPORT_ASSIGN,
	EXPORT_APPEND
}	t_export_op;

/* =========================== */
/*          BUILTINS           */
/* =========================== */

/* src/builtins/pwd.c */
int			builtin_pwd(char **tokens, t_shell *data);

/* src/builtins/env.c */
int			builtin_env(char **tokens, t_shell *data);

/* src/builtins/export.c */
int			set_env_node(t_list **env_list, const char *token);
int			builtin_export(char **tokens, t_shell *data);

/* src/builtins/export_array.c */
t_env		**export_list_to_array(t_list *list, int *size);
void		print_sorted_export(t_env **array, int size);

/* src/builtins/export_sort.c */
void		quicksort(t_env **array, int low, int high);
void		insertion_sort_env(t_env **array, int size);
void		sort_export_array(t_env **array, int size);

/* src/builtins/export_sort_utils.c */
int			median_of_three(t_env **array, int low, int high);
void		swap_env(t_env **a, t_env **b);
int			partition(t_env **array, int low, int high);

/* src/export_update.c */
t_env		*create_new_env_node(char *key, const char *token, t_export_op op);
int			update_existing_env_node(t_env *env_node, const char *token);
int			append_env_value(t_env *env_node, const char *token);

/* src/builtins/export_utils.c */
t_export_op	detect_operation(const char *token);
bool		is_valid_key(const char *token);
char		*get_env_key(const char *token);
char		*get_env_value(const char *token);
t_env		*get_env_node_by_key(t_list *env_list, const char *key);

/* src/builtins/echo.c */
int			builtin_echo(char **tokens, t_shell *data);

/* src/builtins/exit.c */
int			builtin_exit(char **tokens, t_shell *data);

/* =========================== */
/*     ENVIRONMENT IMPORT      */
/* =========================== */

/* src/env/env_import.c      */
void		del_env(void *content);
int			print_env_list(t_list *env_list);
t_env		*create_env_node(const char *str);
t_list		*init_env_from_envp(char **envp);

/* =========================== */
/*            CORE             */
/* =========================== */

/* ascii_art_themes.c */
void		print_ascii_art_hell_simple(void);
void		print_ascii_art_green_breach(void);
void		print_ascii_art_fire_colors(void);
void		print_ascii_art_easter_egg(void);
void		print_ascii_art_dark(void);

/* src/core/minishell_loop.c */
int			minishell_loop(t_shell *data);
bool		prompt_user(char *prompt, t_shell *data);
int			process_line(char *line, t_shell *data);

/* src/core/execute_tokenizer */
char		**execute_tokenizer(char *line, t_shell *data);
bool		validate_tokens(char **tokens, char *line);

/* src/core/execute_builtins.c */
int			execute_builtin(char **tokens, t_shell *data);

/* src/core/init_shell.c */
int			init_shell(t_shell *data, char **envp);

/* src/core/print_ascii_art.c */
bool		is_easter_egg(char *line);
void		display_easter_egg(void);
void		select_random_ascii_art(void);

/* =========================== */
/*           PARSER            */
/* =========================== */

/* src/parser/tokenizer_utils.c */
t_quote		update_quote_state(t_quote current_quote_state, char c);
bool		is_a_shell_separator(t_quote current_quote_state, char c);
bool		has_unclosed_quotes(char const *s);
void		skip_whitespace(char const *s, size_t *i);

/* src/parser/tokenizer_count_tokens.c */
int			count_shell_tokens(const char *s);

/* src/parser/tokenizer_smart_split.c */
char		**ft_split_tokens(char const *s, t_token_error *error_code);

/* =========================== */
/*         EXECUTION           */
/* =========================== */
/* TODO: add execution module functions when implemented */

/* =========================== */
/*           UTILS             */
/* =========================== */

/* src/utils/is_whitespace.c */
bool		is_whitespace(char c);

/* src/utils/memory_cleanup.c */
void		free_string_array(char **tab, size_t count);
void		cleanup_shell(t_shell *data);
void		cleanup_process_line(char **tokens, char *line);

/* src/utils/print_errors_multi.c */
void		print_error_multi(char *part1, char *part2, char *part3,
				char *part4);

/* src/utils/print_error.c */
void		print_error(char *pre_msg, char *main_msg);

#endif
