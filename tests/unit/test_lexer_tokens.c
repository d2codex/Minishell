#include "minishell.h"

void	print_token_type_list(t_token *list)
{
	int		i;
	char	*op_str;

	i = 0;
	while (list)
	{
		if (list->op_type == OP_PIPE)
			op_str = "OP_PIPE";
		else if (list->op_type == OP_INPUT)
			op_str = "OP_INPUT";
		else if (list->op_type == OP_OUTPUT)
			op_str = "OP_OUTPUT";
		else if (list->op_type == OP_APPEND)
			op_str = "OP_APPEND";
		else if (list->op_type == OP_HEREDOC)
			op_str = "OP_HEREDOC";
		else
			op_str = "OP_NONE";

		printf("  Token[%d] value: %s%s%s\n", i,YEL, list->value, RESET);
		if (list->op_type == OP_NONE)
			printf("      Token type: %sTOKEN_WORD%s\n", BLU, RESET);
		else
			printf("      Token type: %sTOKEN_OPERATOR%s\n", GRN, RESET);
		printf("   Operator type: %s\n\n", op_str);
		list = list->next;
		i++;
	}
}

int	main(void)
{
	char	*tokens[] = {"echo", ">", "-n", "<<", "hello", ">>", "|", "wc", "<", NULL};
	t_token	*list;
	int		i;

	printf("     TOKENS LIST: ");
	i = 0;
	while (tokens[i])
	{
		printf("%s%s%s ", YEL, tokens[i], RESET);
		i++;
	}
	printf("\n");
	list = create_token_type_list(tokens);
	if (!list)
	{
		printf("Failed to assign token types\n");
		return (1);
	}
	print_token_type_list(list);
	free_tokens_list(list);
	return (0);
}
