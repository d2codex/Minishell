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

// Helper: Create a test shell
t_shell *create_test_shell(void)
{
	t_shell *data = malloc(sizeof(t_shell));
	if (!data)
		return (NULL);

	data->env_list = NULL;
	data->status = 42;

	set_env_node(&data->env_list, "USER=toto");
	set_env_node(&data->env_list, "HOME=/home/toto");
	set_env_node(&data->env_list, "EMPTY=");

	return (data);
}

void free_test_shell(t_shell *data)
{
	if (data->env_list)
		ft_lstclear(&data->env_list, del_env);
	free(data);
}

// ============ TESTS ============

void test_get_exit_status(void)
{
	printf(CYN "Test 1: Get exit status ($?)... " RESET);

	t_shell *data = create_test_shell();
	char *result = get_var_value("?", data);

	// Check result
	if (result == NULL)
	{
		printf(RED "FAIL: result is NULL\n" RESET);
		return;
	}
	if (strcmp(result, "42") != 0)
	{
		printf(RED "FAIL: expected '42', got '%s'\n" RESET, result);
		free(result);
		free_test_shell(data);
		return;
	}

	free(result);
	free_test_shell(data);
	printf(GRN "PASS\n" RESET);
}

void test_get_existing_variable(void)
{
	printf(CYN "Test 2: Get existing variable (USER)... " RESET);

	t_shell *data = create_test_shell();
	char *result = get_var_value("USER", data);

	if (result == NULL)
	{
		printf(RED "FAIL: result is NULL\n" RESET);
		free_test_shell(data);
		return;
	}
	if (strcmp(result, "toto") != 0)
	{
		printf(RED "FAIL: expected 'toto', got '%s'\n" RESET, result);
		free(result);
		free_test_shell(data);
		return;
	}

	free(result);
	free_test_shell(data);
	printf(GRN "PASS\n" RESET);
}

void test_get_undefined_variable(void)
{
	printf(CYN "Test 3: Get undefined variable... " RESET);

	t_shell *data = create_test_shell();
	char *result = get_var_value("UNDEFINED_VAR", data);

	if (result == NULL)
	{
		printf(RED "FAIL: result is NULL (should return empty string)\n" RESET);
		free_test_shell(data);
		return;
	}
	if (strcmp(result, "") != 0)
	{
		printf(RED "FAIL: expected empty string, got '%s'\n" RESET, result);
		free(result);
		free_test_shell(data);
		return;
	}

	free(result);
	free_test_shell(data);
	printf(GRN "PASS\n" RESET);
}

void test_get_empty_value(void)
{
	printf(CYN "Test 4: Get variable with empty value... " RESET);

	t_shell *data = create_test_shell();
	char *result = get_var_value("EMPTY", data);

	if (result == NULL)
	{
		printf(RED "FAIL: result is NULL\n" RESET);
		free_test_shell(data);
		return;
	}
	if (strcmp(result, "") != 0)
	{
		printf(RED "FAIL: expected empty string, got '%s'\n" RESET, result);
		free(result);
		free_test_shell(data);
		return;
	}

	free(result);
	free_test_shell(data);
	printf(GRN "PASS\n" RESET);
}

void test_null_safety(void)
{
	printf(CYN "Test 5: NULL safety... " RESET);

	t_shell *data = create_test_shell();
	char *result = get_var_value(NULL, data);

	if (result == NULL)
	{
		printf(RED "FAIL: should handle NULL gracefully\n" RESET);
		free_test_shell(data);
		return;
	}

	free(result);
	free_test_shell(data);
	printf(GRN "PASS\n" RESET);
}

int main(void)
{
	printf("\n" BR_CYN "=== Testing get_var_value() ===" RESET "\n\n");

	test_get_exit_status();
	test_get_existing_variable();
	test_get_undefined_variable();
	test_get_empty_value();
	test_null_safety();

	printf("\n" BR_CYN "=== Tests complete ===" RESET "\n\n");
	return (0);
}
