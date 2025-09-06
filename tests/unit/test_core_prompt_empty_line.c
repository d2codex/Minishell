#include "minishell.h"

/**
* @brief Test prompt_user() with an empty line.
*
* Checks that pressing ENTER makes prompt_user() return true instead of exiting.
*
* Compile: make build TEST=unit/test_core_prompt_empty_line.c
* Run:		./bin/test_core_prompt_empty_line
* valgrind (ignore leaks from readline library):
*			make valgrind TEST=unit/test_core_prompt_empty_line.c
*
* Expected:
* - Prompt shows up
* - User presses ENTER
* - Function returns true
*/
void test_prompt_empty_line(void)
{
	printf(BR_BLU "TEST 1 - prompt_user test to check empty line behavior \n" RESET);
	printf(YEL "press any key to start and press ENTER\n" RESET);
	printf(YEL "expected: should return true and be ready for next shell loop iteration (no loop here)\n" RESET);

	getchar();
	bool result = prompt_user("this_is_a_prompt$$$> ", NULL);
	if (result)
		printf(GRN "test passed, returned true as expected\n" RESET);
	else
		printf(RED "test returned false, should be true for empty line\n" RESET);
}

int main(void)
{
	printf(MAG "=== TEST SIMULATE PROMPT + EMPTY LINE ===\n\n" RESET);
	test_prompt_empty_line();
	return (0);
}
