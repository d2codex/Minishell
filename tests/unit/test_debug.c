#include "minishell.h"

int main(void)
{
    char *test1 = ft_substr("echo>'file name'|grep\"pattern\">>out", 0, 4);
    char *test2 = ft_substr("echo>'file name'|grep\"pattern\">>out", 5, 1);
    char *test3 = ft_substr("echo>'file name'|grep\"pattern\">>out", 6, 11);

    printf("test1: %p -> \"%s\"\n", (void*)test1, test1);
    printf("test2: %p -> \"%s\"\n", (void*)test2, test2);
    printf("test3: %p -> \"%s\"\n", (void*)test3, test3);

    printf("About to free test1...\n");
    free(test1);
    printf("About to free test2...\n");
    free(test2);
    printf("About to free test3...\n");
    free(test3);

    printf("All freed successfully\n");
    return 0;
}
