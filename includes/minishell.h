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
typedef enum e_quote
{
	STATE_NORMAL,
	STATE_IN_SINGLE_QUOTE,
	STATE_IN_DOUBLE_QUOTE
}	t_quote;

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
int		is_whitespace(char c); // will move to utils
bool	is_a_shell_separator(t_quote current_quote_state, char c);
int		count_shell_tokens(char const *s);
bool	has_unclosed_quotes(char const *s); // this one is for later
void	print_error(char *pre_msg, char *main_msg); // update this one

/* src/parser/tokeniser_smart_split.c */
void	free_tab(char **tab, int count); // will move to utils
char	**split_shell_tokens(const char *s, char **tab);
char	**ft_split_tokens(char const *s);


#endif
