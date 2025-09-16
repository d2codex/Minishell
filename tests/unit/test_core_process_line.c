#include "minishell.h"

/**
* @brief Test process_line() with different kinds of input.
*
* Checks that input is print back and memory is handled correctly.
* Covers: normal strings, empty line, whitespace, special chars and long line.
*
* Compile:	make build TEST=unit/test_core_process_line.c
* Run:		./bin/test_core_process_line
* valgrind (ignore leaks from readline library):
* 			make valgrind TEST=unit/test_core_process_line.c
*/
void test_process_line()
{
	char *inputs[] = {
		"hello",
		"",
		"[ \t	 \t]",
		"h$w@r|d!#",
		NULL,
	};

	char *descriptions[] = {
		"normal case",
		"empty line",
		"spaces and tabs sequence",
		"special chars",
		"very long line (1000 'A' chars)",
		NULL,
	};

	int num_tests = 5;
	int i = 0;

	while (i < num_tests)
	{
		printf(BR_BLU "*** TEST %d - %s ***\n" RESET, i + 1, descriptions[i]);

		if (i == 1)
			printf(YEL "use cat -e to check the empty line symbol($)\n(but you will also see all the colors related chars as well)" RESET);

		char *line;
		if (inputs[i] == NULL) {
			line = malloc(1001);
			ft_memset(line, 'A', 1000);
			line[1000] = '\0';
		} else {
			line = ft_strdup(inputs[i]);
		}
		process_line(line, NULL);
		printf("\n");
		i++;
	}
}

int	main(void)
{
	printf(MAG "=== TEST PROCESS LINE ===\n\n" RESET);
	test_process_line();
	return (0);
}
