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

/* minishell prompt */
# define SHELL_PROMPT "[mini$HELL] "

/* errors messages tools */
# define ERR_PREFIX "[mini$HELL]: "
# define ERR_CD "cd: "
# define ERR_EXIT "exit: "
# define ERR_ENV "env: "
# define ERR_EXPORT "export: `"
# define ERR_NOT_VALID_ID "': not a valid identifier"
# define ERR_NUMERIC_ARG ": numeric argument required"
# define ERR_TOO_MANY_ARGS "too many arguments"
# define ERR_SYNTAX "syntax error near unexpected token `"

/* easter egg */
# define EASTER_EGG "101010"

/* standardized return codes */
// continue to use EXIT_FAILURE 0
// and EXIT_SUCCESS 1
# define MISUSAGE_ERROR 2 // misuse error (ex invalid key)
# define INTERNAL_ERROR 125 // shell internal failure
# define CMD_NOT_FOUND 127 // external command not found
// add more here for other codes

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
	bool	should_exit; //new flag for main loop
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

/* enum to track the quote current state, used inside the tokenizer */
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

/* detects operation: assign, append or key only mode, used in export builtin */
typedef enum e_export_op
{
	EXPORT_NONE,
	EXPORT_ASSIGN,
	EXPORT_APPEND
}	t_export_op;

/* =========================== */
/*           LEXER             */
/* =========================== */
typedef enum t_token_type
{
	TOKEN_WORD,
	TOKEN_OPERATOR
}	t_token_type;

typedef enum t_node_type
{
	NODE_NONE,
	NODE_CMD,
	NODE_PIPE,
	NODE_REDIR
}	t_node_type;

typedef enum e_operator_type
{
	OP_NONE,
	OP_PIPE,
	OP_INPUT,
	OP_OUTPUT,
	OP_APPEND,
	OP_HEREDOC
	// if we have time for bonus later
	// OP_AND,
	// OP_OR,
	// OP_PAREN_OPEN,
	// OP_PAREN_CLOSE
}	t_operator_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	t_operator_type	op_type;
	struct s_token	*next;
}	t_token;

// unused fields will be set to 0 or null
typedef struct s_ast
{
	t_node_type		type; // NODE_CMD, NODE_PIPE or NODE_REDIR
	char			*value; // raw token string (cmd or word)
	char			**argv; // only for NODE_CMD
	char			*filename; // only for NODE_REDIR
	t_operator_type	op_type;// <, >, >>, <<
	struct s_ast	*left; // pipe left
	struct s_ast	*right; // pipe right
	struct s_ast	*next; // used temporarily for flat list
							//
}	t_ast;

/* =========================== */
/*          BUILTINS           */
/* =========================== */

/* src/builtins/pwd.c */
int			builtin_pwd(char **tokens, t_shell *data);

/* src/builtins/cd.c */
int			builtin_cd(char **tokens, t_shell *data);

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

/* src/builtins/unset.c */
int			remove_env_node(t_list **env_list, const char *token);
int			builtin_unset(char **tokens, t_shell *data);

/* src/builtins/execute_builtins.c */
bool		execute_builtin(char **tokens, t_shell *data);

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

/* src/core/ascii_art_themes.c */
void		print_ascii_art_hell_simple(void);
void		print_ascii_art_green_breach(void);
void		print_ascii_art_fire_colors(void);
void		print_ascii_art_easter_egg(void);
void		print_ascii_art_dark(void);

/* src/core/minishell_loop.c */
int			minishell_loop(t_shell *data);
bool		prompt_user(char *prompt, t_shell *data);
int			process_line(char *line, t_shell *data);

/* src/core/init_shell.c */
int			init_shell(t_shell *data, char **envp);

/* src/core/print_ascii_art.c */
bool		is_easter_egg(char *line);
void		display_easter_egg(void);
void		select_random_ascii_art(void);

/* =========================== */
/*           PARSER            */
/* =========================== */

/* src/parser/assign_argv_and_filename.c */
int			assign_argv_and_filename(t_ast *ast_list);

/* src/parser/categorize_ast.c */
void		free_ast_list(t_ast *list);
t_ast		*new_ast_node(t_token *token);
bool		is_a_redirection(t_operator_type op_type);
void		assign_ast_node_type(t_ast *ast_list);
t_ast		*create_ast_list(t_token *tokens_list);

/* src/parser/categorize_tokens.c */
int			get_operator_type(char *token);
void		free_tokens_list(t_token *head);
t_token		*create_token_type_list(char **tokens);

/* src/parser/execute_tokenizer */
char		**execute_tokenizer(char *line, t_shell *data);
bool		validate_tokens(char **tokens);

/* src/parser/tokenizer_utils.c */
t_quote		update_quote_state(t_quote current_quote_state, char c);
bool		is_a_shell_separator(t_quote current_quote_state, char c);
bool		has_unclosed_quotes(char const *s);
void		skip_whitespace(char const *s, size_t *i);

/* src/parser/tokenizer_count_tokens.c */
int			count_shell_tokens(const char *s);

/* src/parser/tokenizer_smart_split.c */
char		**ft_split_tokens(char const *s, t_token_error *error_code);

/* src/parser/validate_syntax.c */
int			validate_syntax_ast_list(t_ast *list);

/* =========================== */
/*         EXPANSION           */
/* =========================== */

/* src/expansion/expansion_extract.c */
char 		*extract_var_name(const char *str, size_t start_pos);
char 		*get_var_value(const char *var_name, t_shell *data);

/* src/expansion/expansion_integrate.c */
void		expand_ast_nodes(t_ast *ast_list, t_shell *data);
void		sync_tokens_with_ast(char **tokens, t_ast *ast_list);

/* src/expansion/expansion_replace.c */
size_t		calculate_expanded_size(const char *str, t_shell *data);
void		fill_expanded_string(char *result, const char *str, t_shell *data);
char		*expand_variables_in_string(const char *str, t_shell *data);

/* src/expansion/expansion_utils.c */
bool 		should_expand_at_position(const char *str, size_t pos);

/* =========================== */
/*         EXECUTION           */
/* =========================== */

/* src/execution/execute.c */
int			execute_external_command(char **tokens, t_shell *data);

/* =========================== */
/*           UTILS             */
/* =========================== */

/* src/utils/is_whitespace.c */
bool		is_whitespace(char c);

/* src/utils/memory_cleanup.c */
void		free_string_array(char **tab, size_t count);
void		cleanup_shell(t_shell *data);
void		cleanup_process_line(char **tokens, t_ast *ast_list,
				t_token *token_list, char *line);

/* src/utils/print_errors.c */
void		print_error(char *p1, char *p2, char *p3, char *p4);

#endif
