#include "minishell.h"

int	main(void)
{
	int		ret;

	// Test ft_putchar_fd
	ret = ft_putchar_fd('A', STDOUT_FILENO);
	printf(" <- ft_putchar_fd returned %d\n", ret);
	// Test ft_putstr_fd with a normal string
	ret = ft_putstr_fd("Hello, world!", STDOUT_FILENO);
	printf(" <- ft_putstr_fd returned %d\n", ret);
    // Test ft_putstr_fd with NULL
	ret = ft_putstr_fd(NULL, STDOUT_FILENO);
	printf("ft_putstr_fd(NULL) returned %d (expected 0)\n", ret);
	// Test ft_putendl_fd
	ret = ft_putendl_fd("This is a line", STDOUT_FILENO);
	printf("ft_putendl_fd returned %d\n", ret);
	// Test ft_putendl_fd with NULL
	ret = ft_putendl_fd(NULL, STDOUT_FILENO);
	printf("ft_putendl_fd(NULL) returned %d (expected 0)\n", ret);
	return (0);
}
