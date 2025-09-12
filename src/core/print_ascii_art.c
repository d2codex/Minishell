#include "minishell.h"

/* note: this is a first draft, I will add colors and shiny stuffs later + your
ideas for the song if possible :D */
/* void	print_ascii_art(void)
{
	printf("🌠 Welcome to 🌠\n");
	printf(CYN "▄▄███▄▄·██╗  ██╗███████╗██╗     ██╗\n" RESET);
	printf(CYN "██╔════╝██║  ██║██╔════╝██║     ██║\n" RESET);
	printf(CYN "███████╗███████║█████╗  ██║     ██║\n" RESET);
	printf(CYN "╚════██║██╔══██║██╔══╝  ██║     ██║ \n" RESET);
	printf(CYN "███████║██║  ██║███████╗███████╗███████╗ \n" RESET);
	printf(CYN "╚═▀▀▀══╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n" RESET);
	printf("such a lovely place, such a lovely face...\n");
} */
// uncomment if you want to test this one, comment the one above.
// Don't change alignement, it works (color codes mess up the alignment)
void	print_ascii_art(void)
{
	printf(BR_GRN "> SYSTEM BREACH DETECTED\n" RESET);
	printf(BR_GRN "> ACCESSING $HELL TERMINAL...\n" RESET);
	printf(BR_GRN "> " BR_RED "ERR 666: " RESET BR_GRN "WELCOME TO\n" RESET);
	printf("\n");
	printf(BR_RED "   $" BR_YEL "██╗  ██╗███████╗██╗     ██╗\n" RESET);
	printf(BR_RED "   $" BR_YEL "██║  ██║██╔════╝██║     ██║\n" RESET);
	printf(BR_YEL "   $" GRN "███████║█████╗  ██║     ██║\n" RESET);
	printf(BR_YEL "   $" GRN "██╔══██║██╔══╝  ██║     ██║\n" RESET);
	printf(GRN "   $" BR_GRN "██║  ██║███████╗███████╗███████╗\n" RESET);
	printf(GRN "   $" BR_GRN "╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n" RESET);
	printf("\n");
	printf(BR_GRN "> REMEMBER: " RESET "\"You can check out any time..\"\n");
	printf(BR_RED "> WARNING: " RESET "\"But you can never leave!\"\n");
}
