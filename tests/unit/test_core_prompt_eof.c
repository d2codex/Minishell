#include "minishell.h"

/**
* @brief Test prompt_user() with EOF (Ctrl+D).
*
* Checks that EOF makes prompt_user() print "exit" and return false.
*
* Compile:	make build TEST=unit/test_core_prompt_eof.c
* Run:		./bin/test_core_prompt_eof
* valgrind (ignore leaks from readline library):
*			make valgrind TEST=unit/test_core_prompt_eof.c
*
* Expected:
* - Prompt shows up
* - User presses Ctrl+D
* - Function prints "exit" and returns false
*/
void	test_prompt_user_eof(void)
{
	printf(BR_BLU "TEST 1 - prompt_user test to simulate EOF action \n" RESET);
	printf(YEL "press any key to start and then press CTRL+D\n" RESET);
	printf(YEL "expected: should print 'exit' and return false\n" RESET);

	getchar();
	bool result = prompt_user("this_is_a_prompt$$$> ", NULL);
	if (!result)
		printf(GRN "test passed, 'exit' printed and returned false as expected\n" RESET);
	else
		printf(RED "test returned true, make sure you press CTRL+D\n" RESET);
}

int main(void)
{
	printf(MAG "=== TEST SIMULATE PROMPT + EOF ===\n\n" RESET);
	test_prompt_user_eof();
	return (0);
}
