#include "minishell.h"

// TODO: to be deleted later, just a function so i can test expansion for now
static char	**token_list_to_argv(t_token *list)
{
	int		i;
	int		count;
	t_token	*curr;
	char	**argv;

	count = 0;
	curr = list;
	while (curr && curr->type == TOKEN_WORD)
	{
		count++;
		curr = curr->next;
	}
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	curr = list;
	i = 0;
	while (i < count)
	{
		argv[i] = ft_strdup(curr->value);
		curr = curr->next;
		i++;
	}
	argv[count] = NULL;
	return (argv);
}

// TODO: deleted function after ast tree is correctly built
// and execute_builtin has the proper function signature
static void	free_argv(char **argv)
{
	int	i;

	i = 0;
	if (!argv)
		return ;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

/**
 * @brief Check and execute a builtin command.
 *
 * Compares the first token against the list of supported builtins,
 * including `exit`. If a match is found, executes the corresponding
 * function and updates `data->status`. Does not execute external commands.
 *
 * @param argv Command argv from user input.
 * @param data Shell state, including environment, exit status, and exit flag.
 * @return true if the command is a builtin and was executed; false otherwise.
 * TODO: update comment and update the function prototype to accept
 * the AST once it is built.
 * This is just temporary so taht we can test
 */
bool	execute_builtin(t_token **token_list, t_shell *data)
{
	int						i;
	static const t_builtin	builtins[] = {
	{"pwd", builtin_pwd}, {"export", builtin_export},
	{"exit", builtin_exit}, {"echo", builtin_echo},
	{"env", builtin_env}, {"unset", builtin_unset},
	{"cd", builtin_cd},
	{NULL, NULL}};

	if (!token_list || !*token_list || !(*token_list)->value)
		return (false);
	i = 0;
	while (builtins[i].cmd != NULL)
	{
		if (ft_strcmp((*token_list)->value, builtins[i].cmd) == 0)
		{
			// TODO: delete this after we ahve the actual ast tree
			char **argv = token_list_to_argv(*token_list);	
			builtins[i].f(argv, data);
			free_argv(argv);
			return (true);
		}
		i++;
	}
	return (false);
}
