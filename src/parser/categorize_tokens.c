#include "minishell.h"

/**
 * @brief Get the operator type of a token.
 *
 * Determines if a given token is an operator
 * (pipe, redirection, heredoc, etc.).
 *
 * @param token The token string to analyze.
 * @return t_operator_type Enum value for the operator type,
 *         or OP_NONE if not an operator.
 */
int	get_operator_type(char *token)
{
	if (!token || !*token)
		return (OP_NONE);
	if (*token == '|' && token[1] == '\0')
		return (OP_PIPE);
	if (*token == '<' && token[1] == '\0')
		return (OP_INPUT);
	if (*token == '>' && token[1] == '\0')
		return (OP_OUTPUT);
	if (*token == '<' && token[1] == '<' && token[2] == '\0')
		return (OP_HEREDOC);
	if (*token == '>' && token[1] == '>' && token[2] == '\0')
		return (OP_APPEND);
	// if we do bonus
	// if (*token == '&' && token[1] == '&' && token[2] == '\0')
	//	 return (OP_AND);
	// if (*token == '|' && token[1] == '|' && token[2] == '\0')
	//	 return (OP_OR);
	// if (*token == '(' && token[1] == '\0')
	//	 return (OP_PAREN_OPEN);
	// if (*token == ')' && token[1] == '\0')
	//	 return (OP_PAREN_CLOSE);
	return (OP_NONE);
}

/**
 * @brief Free a linked list of tokens.
 *
 * Iterates through the list, freeing each token's
 * string value and the token node itself.
 *
 * @param list Pointer to the head of the token list.
 */
void	free_tokens_list(t_token *list)
{
	t_token	*current;

	while (list)
	{
		current = list;
		list = list->next;
		if (current->value)
			free(current->value);
		free(current);
	}
}

// function that mallocs and set node,
// malloc_and_node haha like lock and load
/**
 * @brief Allocate and initialize a new token node.
 *
 * Duplicates the token string, determines its operator type,
 * and sets the token type accordingly.
 *
 * @param token Input string representing the token.
 * @return Pointer to the new token node, or NULL on failure.
 */
static t_token	*new_token_node(char *token)
{
	t_token	*new;

	if (!token)
		return (NULL);
	new = malloc(sizeof (t_token));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->value = ft_strdup(token);
	if (!new->value)
	{
		free(new);
		return (NULL);
	}
	new->op_type = get_operator_type(token);
	if (new->op_type == OP_NONE)
		new->type = TOKEN_WORD;
	else
		new->type = TOKEN_OPERATOR;
	return (new);
}

/**
 * @brief Build a linked list of tokens from a null-terminated
 * array of strings.
 *
 * Each token is converted into a t_token node (with type/op_type)
 * and appended to the list using a tail pointer (0(1) per append).
 * On allocation failure, all previously created nodes are freed.
 *
 * @param tokens Null-terminated array of strings (raw tokens).
 * @return Head of the token list, or NULL on failure.
 */
t_token	*create_token_type_list(char **tokens)
{
	t_token	*new;
	t_token	*tokens_list;
	t_token	*tail;
	int		i;

	tokens_list = NULL;
	tail = NULL;
	i = 0;
	while (tokens[i])
	{
		new = new_token_node(tokens[i]);
		if (!new)
		{
			free_tokens_list(tokens_list);
			return (NULL);
		}
		if (!tokens_list) //no list yet
			tokens_list = new;
		else
			tail->next = new;
		tail = new;
		i++;
	}
	return (tokens_list);
}
