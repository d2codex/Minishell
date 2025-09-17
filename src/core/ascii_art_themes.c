#include "minishell.h"

void	print_ascii_art_hell_simple(void)
{
	printf(" > welcome to\n");
	printf(CYN "▄▄███▄▄·██╗  ██╗███████╗██╗	 ██╗\n" RESET);
	printf(CYN "██╔════╝██║  ██║██╔════╝██║	 ██║\n" RESET);
	printf(CYN "███████╗███████║█████╗  ██║	 ██║\n" RESET);
	printf(CYN "╚════██║██╔══██║██╔══╝  ██║	 ██║ \n" RESET);
	printf(CYN "███████║██║  ██║███████╗███████╗███████╗ \n" RESET);
	printf(CYN "╚═▀▀▀══╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n" RESET);
	printf("such a lovely place, such a lovely interface...\n");
}

void	print_ascii_art_green_breach(void)
{
	printf(BR_GRN "> SYSTEM BREACH DETECTED\n" RESET);
	printf(BR_GRN "> ACCESSING $HELL TERMINAL...\n" RESET);
	printf(BR_GRN "> " BR_RED "ERR 666: " RESET BR_GRN "WELCOME TO\n" RESET);
	printf("\n");
	printf(BR_RED "   $" BR_YEL "██╗  ██╗███████╗██╗	 ██╗\n" RESET);
	printf(BR_RED "   $" BR_YEL "██║  ██║██╔════╝██║	 ██║\n" RESET);
	printf(BR_YEL "   $" GRN "███████║█████╗  ██║	 ██║\n" RESET);
	printf(BR_YEL "   $" GRN "██╔══██║██╔══╝  ██║	 ██║\n" RESET);
	printf(GRN "   $" BR_GRN "██║  ██║███████╗███████╗███████╗\n" RESET);
	printf(GRN "   $" BR_GRN "╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n" RESET);
	printf("\n");
	printf(BR_GRN "> REMEMBER: " RESET "\"you can check out any time...\"\n");
	printf(BR_RED "> WARNING: " RESET "\"but you can never leave!\"\n");
}

void	print_ascii_art_fire_colors(void)
{
	printf("welcome to \n");
	printf(BR_YEL "▄▄███▄▄·██╗  ██╗███████╗██╗	 ██╗\n" RESET);
	printf(BR_YEL "██╔════╝██║  ██║██╔════╝██║	 ██║\n" RESET);
	printf(YEL	"███████╗███████║█████╗  ██║	 ██║\n" RESET);
	printf(RED	"╚════██║██╔══██║██╔══╝  ██║	 ██║ \n" RESET);
	printf(RED	"███████║██║  ██║███████╗███████╗███████╗ \n" RESET);
	printf(BR_RED "╚═▀▀▀══╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n" RESET);
	printf(YEL "such a lovely place, such a lovely interface...\n" RESET);
}

void	print_ascii_art_easter_egg(void)
{
	printf(BR_GRN "> CONNECTION ESTABLISHED\n" RESET);
	printf(GRN "> LOADING INTERFACE...\n" RESET);
	printf(GRN "> don't panic, type: 101010 in case of emergency\n");
	printf("\n");
	printf(GRN "    $" BR_GRN "██╗  ██╗███████╗██╗     ██╗\n" RESET);
	printf(GRN "    $" BR_GRN "██║  ██║██╔════╝██║     ██║\n" RESET);
	printf(BR_GRN "    $" GRN "███████║█████╗  ██║     ██║\n" RESET);
	printf(BR_GRN "    $" GRN "██╔══██║██╔══╝  ██║     ██║\n" RESET);
	printf(GRN "    $" BR_GRN "██║  ██║███████╗███████╗███████╗\n" RESET);
	printf(GRN "    $" BR_GRN "╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n" RESET);
	printf("\n");
	printf(BR_GRN "> ...\n" RESET);
}

void	print_ascii_art_dark(void)
{
	printf(BR_BLK "> ENTER...\n" RESET);
	printf("\n");
	printf(BLK "   ░▒▓$" BR_RED "██╗  ██╗███████╗██╗     ██╗\n" RESET);
	printf(BLK "   ░▒▓$" RED "██║  ██║██╔════╝██║     ██║\n" RESET);
	printf(BR_BLK "   ░▒▓$" BR_RED "███████║█████╗  ██║     ██║\n" RESET);
	printf(BR_BLK "   ░▒▓$" RED "██╔══██║██╔══╝  ██║     ██║\n" RESET);
	printf(BLK "   ░▒▓$" BR_RED "██║  ██║███████╗███████╗███████╗\n" RESET);
	printf(BLK "   ░▒▓$" BR_RED "╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n" RESET);
	printf("\n");
}
