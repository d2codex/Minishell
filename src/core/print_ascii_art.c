#include "minishell.h"

void	print_ascii_art_hell(void)
{
	printf(" > Welcome to\n");
	printf(CYN "▄▄███▄▄·██╗  ██╗███████╗██╗	 ██╗\n" RESET);
	printf(CYN "██╔════╝██║  ██║██╔════╝██║	 ██║\n" RESET);
	printf(CYN "███████╗███████║█████╗  ██║	 ██║\n" RESET);
	printf(CYN "╚════██║██╔══██║██╔══╝  ██║	 ██║ \n" RESET);
	printf(CYN "███████║██║  ██║███████╗███████╗███████╗ \n" RESET);
	printf(CYN "╚═▀▀▀══╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n" RESET);
	printf("such a lovely place, such a lovely interface...\n");
}

void	print_ascii_art_breach(void)
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
	printf(BR_GRN "> REMEMBER: " RESET "\"You can check out any time..\"\n");
	printf(BR_RED "> WARNING: " RESET "\"But you can never leave!\"\n");
}

void	print_ascii_art_skull(void)
{
	printf("💀 Welcome to 💀\n");
	printf(BR_YEL "▄▄███▄▄·██╗  ██╗███████╗██╗	 ██╗\n" RESET);
	printf(BR_YEL "██╔════╝██║  ██║██╔════╝██║	 ██║\n" RESET);
	printf(YEL	"███████╗███████║█████╗  ██║	 ██║\n" RESET);
	printf(RED	"╚════██║██╔══██║██╔══╝  ██║	 ██║ \n" RESET);
	printf(RED	"███████║██║  ██║███████╗███████╗███████╗ \n" RESET);
	printf(BR_RED "╚═▀▀▀══╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n" RESET);
	printf(YEL "such a lovely place, such a lovely interface...\n" RESET);
}

void	print_ascii_art_matrix(void)
{
	printf(BR_GRN "> CONNECTION ESTABLISHED\n" RESET);
	printf(GRN "> LOADING INTERFACE...\n" RESET);
	printf(GRN "00100100 01101000 01100101 01101100 01101100\n" RESET);
	printf("\n");
	printf(GRN "    ████" BR_GRN "██╗  ██╗███████╗██╗     ██╗\n" RESET);
	printf(GRN "    ████" BR_GRN "██║  ██║██╔════╝██║     ██║\n" RESET);
	printf(BR_GRN "    ████" GRN "███████║█████╗  ██║     ██║\n" RESET);
	printf(BR_GRN "    ████" GRN "██╔══██║██╔══╝  ██║     ██║\n" RESET);
	printf(GRN "    ████" BR_GRN "██║  ██║███████╗███████╗███████╗\n" RESET);
	printf(GRN "    ████" BR_GRN "╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n" RESET);
	printf("\n");
	printf(BR_GRN "> ...\n" RESET);
}

void	print_ascii_art_dark(void)
{
	printf(BR_BLK "> ENTER...\n" RESET);
	printf("\n");
	printf(BLK "   ░▒▓" BR_RED "██╗  ██╗███████╗██╗     ██╗\n" RESET);
	printf(BLK "   ░▒▓" RED "██║  ██║██╔════╝██║     ██║\n" RESET);
	printf(BR_BLK "   ░▒▓" BR_RED "███████║█████╗  ██║     ██║\n" RESET);
	printf(BR_BLK "   ░▒▓" RED "██╔══██║██╔══╝  ██║     ██║\n" RESET);
	printf(BLK "   ░▒▓" BR_RED "██║  ██║███████╗███████╗███████╗\n" RESET);
	printf(BLK "   ░▒▓" BR_RED "╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n" RESET);
	printf("\n");
}

void	print_random_ascii_art(void)
{
	int		dummy;
	void	*ptr;
	int		fd;
	int		choice;

	ptr = malloc(1);
	fd = dup(0);
	choice = (((unsigned long)&dummy) +
			  ((unsigned long)ptr) +
			  fd) % 5;
	free(ptr);
	close(fd);

	if (choice == 0)
		print_ascii_art_hell();
	else if (choice == 1)
		print_ascii_art_breach();
	else if (choice == 2)
		print_ascii_art_skull();
	else if (choice == 3)
		print_ascii_art_matrix();
	else
		print_ascii_art_dark();
}
