#ifndef MINISHELL_H
# define MINISHELL_H

/* =========================== */
/*          INCLUDES           */
/* =========================== */
# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>

/* =========================== */
/*        STRUCTURES           */
/* =========================== */


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
/*            LOOP             */
/* =========================== */

/* minishell_loop.c */
void	process_line(char *line);
int		prompt_user(char *prompt);


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
/*           UTILS             */
/* =========================== */
void	free_string_array(char **tab, size_t count);
bool	is_whitespace(char c);
void	print_error(char *pre_msg, char *main_msg);

#endif
