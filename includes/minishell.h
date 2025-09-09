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

/* =========================== */
/*          BUILTINS           */
/* =========================== */

/* pwd.c */
int		builtin_pwd(char **tokens, t_shell *data);

/* src/builtins/export.c */
int		handle_invalid_key(const char *token);
t_env	*create_new_env_node(char *key, const char *token);
int		update_existing_env_node(t_env *env_node, const char *token);
int		set_env_node(t_list **env_list, const char *token);
int		builtin_export(t_list **env_list, char **tokens);

/* src/builtins/export_array.c */
t_env	**export_list_to_array(t_list *list, int *size);
void	print_sorted_export(t_env **array, int size);

/* src/builtins/export_sort.c */
void	quicksort(t_env **array, int low, int high);
void	insertion_sort_env(t_env **array, int size);
void	sort_export_array(t_env **array, int size);

/* src/builtins/export_sort_utils.c */
int		compare_keys(const char *s1, const char *s2);
int		median_of_three(t_env **array, int low, int high);
void	swap_env(t_env **a, t_env **b);
int		partition(t_env **array, int low, int high);

/* src/builtins/export_utils.c */
bool	has_equal(const char *token);
bool	is_valid_key(const char *token);
char	*get_env_key(const char *token);
char	*get_env_value(const char *token);
t_env	*get_env_node_by_key(t_list *env_list, const char *key);

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

/* src/utils/print_error.c */
void	print_error(char *pre_msg, char *main_msg);

#endif
