#include "minishell.h"

/**
 * @brief check if input is the binary easter egg sequence
 *
 * @param line input line from user
 * @return true if it's the easter egg, false otherwise
 */
bool	is_easter_egg(char *line)
{
	if (!line)
		return (false);
	return (ft_strcmp(line, EASTER_EGG) == 0);
}

/**
 * @brief display easter egg message
 */
void	display_easter_egg(void)
{
	printf(BR_RED " SEQUENCE DETECTED! \n" RESET);
	printf(BR_GRN "   decoding: 101010 = 42\n" RESET);
	printf(BR_YEL "   ┌─────────────────────────────┐\n" RESET);
	printf(BR_YEL "   │ ! you've found the answer ! │\n" RESET);
	printf(BR_YEL "   │    ! outstanding move !     │\n" RESET);
	printf(BR_YEL "   └─────────────────────────────┘\n" RESET);
}
