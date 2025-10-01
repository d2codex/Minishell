#include "minishell.h"

char	*get_var_value(const char *var_name, t_shell *data)
{
	t_env	*env_node;

	// TDD test5 - handle NULL input
	if (!var_name || !data)
		return (ft_strdup(""));

	// TDD test1 - special case, exit status
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(data->status));

	// TDD test2 - test3 - test4, search in env list
	env_node = get_env_node_by_key(data->env_list, var_name);
	if (env_node && env_node->value)
		return (ft_strdup(env_node->value));

	// implicit "else", variable not found, return empty string (bash behavior)
	return (ft_strdup(""));
}

bool	should_expand_at_position(const char *str, size_t pos)
{
	size_t		i;
	t_quote		current_quote_state;

	if (!str || str[pos] != '$')
		return (false);
	i = 0;
	current_quote_state = STATE_NOT_IN_QUOTE;
	while (i <= pos)
	{
		current_quote_state = update_quote_state(current_quote_state, str[i]);
		i++;
	}
	if (current_quote_state == STATE_NOT_IN_QUOTE
		|| current_quote_state == STATE_IN_DOUBLE_QUOTE)
		return (true);
	else
		return (false);
}

char	*extract_var_name(const char *str, size_t start_pos)
{
	int		var_len;
	char	*trimmed_name;
	int		i;
	int		j;

	// Basic validation: ensure string exists and starts with '$'
	if (!str || str[start_pos] != '$')
		return (NULL);

	// Special case: $? expands to exit status
	if (str[start_pos + 1] == '?')
		return (ft_strdup("?"));

	// Special case: $$ expands to process ID (bonus feature)
	/* if (str[start_pos + 1] == '$')
		return (ft_strdup("$")); */

	// Invalid: variable name cannot start with a digit
	// Example: $123 is not a valid variable
	if (ft_isdigit(str[start_pos + 1]))
		return (NULL);

	// Invalid cases:
	// - '$' alone (no character after)
	// - Character after '$' is neither letter nor underscore
	if ((!str[start_pos + 1]) || ((!ft_isalpha(str[start_pos + 1]))
		&& (str[start_pos + 1] != '_')))
		return (NULL);

	// Calculate variable name length
	// Start from position after '$' and continue while characters are valid
	// Valid characters: letters (a-z, A-Z), digits (0-9), underscore (_)
	i = start_pos + 1;
	while (str[i])
	{
		// Stop at first character that's not alphanumeric or underscore
		if (!ft_isalpha(str[i]) && !ft_isdigit(str[i]) && (str[i] != '_'))
			break ;
		i++;
	}

	// Calculate length: distance from start to end, minus the '$' itself
	// Example: "$USER" → i=5, start_pos=0 → length = 5 - 0 - 1 = 4
	var_len = i - start_pos - 1;

	// Allocate memory for the variable name (without the '$')
	trimmed_name = malloc((sizeof(char) * var_len) + 1);
	if (!trimmed_name)
		return (NULL);

	// Copy the variable name character by character
	// Start from position after '$' (start_pos + 1)
	// Copy exactly var_len characters (the variable name length)
	j = 0;
	i = start_pos + 1;
	while (j < var_len)
	{
		trimmed_name[j] = str[i];
		j++;
		i++;
	}

	// Null-terminate the string
	trimmed_name[j] = '\0';

	return (trimmed_name);
}

static size_t	get_variable_size(const char *str, size_t *i, t_shell *data)
{
	char	*var_name;
	char	*value;
	size_t	var_size;
	size_t	name_len;

	var_name = extract_var_name(str, *i);
	if (!var_name)
		return (0);
	value = get_var_value(var_name, data);
	var_size = ft_strlen(value);
	name_len = ft_strlen(var_name);
	*i += name_len + 1;
	free(var_name);
	free(value);
	return (var_size);
}

size_t	calculate_expanded_size(const char *str, t_shell *data)
{
	size_t	total_size;
	size_t	i;
	size_t	var_size;

	total_size = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && should_expand_at_position(str, i))
		{
			var_size = get_variable_size(str, &i, data);
			total_size += var_size;
		}
		else
		{
			total_size++;
			i++;
		}
	}
	return (total_size);
}

static size_t	copy_variable_value(char *result, size_t dst_pos,
	const char *var_name, t_shell *data)
{
	char	*var_value;
	size_t	j;

	var_value = get_var_value(var_name, data);
	j = 0;
	while (var_value[j])
	{
		result[dst_pos + j] = var_value[j];
		j++;
	}
	free(var_value);
	return (j);
}

void	fill_expanded_string(char *result, const char *str, t_shell *data)
{
	size_t	src_pos;
	size_t	dst_pos;
	char	*var_name;

	src_pos = 0;
	dst_pos = 0;
	while (str[src_pos])
	{
		if (str[src_pos] == '$' && should_expand_at_position(str, src_pos))
		{
			var_name = extract_var_name(str, src_pos);
			if (!var_name)
				return ;
			dst_pos += copy_variable_value(result, dst_pos, var_name, data);
			src_pos += ft_strlen(var_name) + 1;
			free(var_name);
		}
		else
		{
			result[dst_pos] = str[src_pos];
			dst_pos++;
			src_pos++;
		}
	}
	result[dst_pos] = '\0';
}

char	*expand_variables_in_string(const char *str, t_shell *data)
{
	char	*result;
	size_t	final_size;

	if (!str || !data)
		return (NULL);
	final_size = calculate_expanded_size(str, data);
	if (final_size == 0)
		return (ft_strdup(""));
	result = malloc(sizeof(char) * (final_size + 1));
	if (!result)
		return (NULL);
	fill_expanded_string(result, str, data);
	return (result);
}

/* ************************************************************************** */

t_shell *create_test_shell(void)
{
	t_shell *data = malloc(sizeof(t_shell));

	data->env_list = NULL;
	data->status = 42;
	data->is_tty = false;
	data->is_child = false;
	data->should_exit = false;

	set_env_node(&data->env_list, "USER=toto");
	set_env_node(&data->env_list, "HOME=/home/user");
	set_env_node(&data->env_list, "EMPTY=");

	return (data);
}

void free_test_shell(t_shell *data)
{
	if (data->env_list)
		ft_lstclear(&data->env_list, del_env);
	free(data);
}

void test_basic_expansion(void)
{
	printf(CYN "Test 1: Basic variable expansion... " RESET);

	t_shell *data = create_test_shell();
	char *result = expand_variables_in_string("hello $USER world", data);

	if (!result || strcmp(result, "hello toto world") != 0)
		printf(RED "FAIL: expected 'hello toto world', got '%s'\n" RESET, result ? result : "NULL");
	else
		printf(GRN "PASS\n" RESET);

	if (result) free(result);
	free_test_shell(data);
}

void test_multiple_variables(void)
{
	printf(CYN "Test 2: Multiple variables... " RESET);

	t_shell *data = create_test_shell();
	char *result = expand_variables_in_string("$USER lives in $HOME", data);

	if (!result || strcmp(result, "toto lives in /home/user") != 0)
		printf(RED "FAIL: expected 'toto lives in /home/user', got '%s'\n" RESET, result ? result : "NULL");
	else
		printf(GRN "PASS\n" RESET);

	if (result) free(result);
	free_test_shell(data);
}

void test_exit_status(void)
{
	printf(CYN "Test 3: Exit status ($?)... " RESET);

	t_shell *data = create_test_shell();
	char *result = expand_variables_in_string("exit code: $?", data);

	if (!result || strcmp(result, "exit code: 42") != 0)
		printf(RED "FAIL: expected 'exit code: 42', got '%s'\n" RESET, result ? result : "NULL");
	else
		printf(GRN "PASS\n" RESET);

	if (result) free(result);
	free_test_shell(data);
}

void test_quotes(void)
{
	printf(CYN "Test 4: Quote handling... " RESET);

	t_shell *data = create_test_shell();
	char *result = expand_variables_in_string("'$USER' and \"$HOME\"", data);

	if (!result || strcmp(result, "'$USER' and \"/home/user\"") != 0)
		printf(RED "FAIL: expected ''$USER' and \"/home/user\"', got '%s'\n" RESET, result ? result : "NULL");
	else
		printf(GRN "PASS\n" RESET);

	if (result) free(result);
	free_test_shell(data);
}

void test_undefined_variable(void)
{
	printf(CYN "Test 5: Undefined variable... " RESET);

	t_shell *data = create_test_shell();
	char *result = expand_variables_in_string("hello $UNDEFINED", data);

	if (!result || strcmp(result, "hello ") != 0)
		printf(RED "FAIL: expected 'hello ', got '%s'\n" RESET, result ? result : "NULL");
	else
		printf(GRN "PASS\n" RESET);

	if (result) free(result);
	free_test_shell(data);
}

int main(void)
{
	printf("\n" BR_CYN "=== Testing complete variable expansion ===" RESET "\n\n");

	test_basic_expansion();
	test_multiple_variables();
	test_exit_status();
	test_quotes();
	test_undefined_variable();

	printf("\n" BR_CYN "=== Tests complete ===" RESET "\n\n");
	return (0);
}
