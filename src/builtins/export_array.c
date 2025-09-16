#include "minishell.h"

/**
 * @brief Convert a linked list of t_env nodes into an array.
 *
 * Allocates an array of pointers to t_env structs from the given linked
 * list. The array is NULL-terminated. Caller is responsible for freeing
 * the array (but not the t_env nodes themselves).
 *
 * @param list Pointer to the head of the t_env linked list.
 * @param size Pointer to an int where the array size (number of elements)
 *             will be stored.
 * @return t_env** Array of t_env pointers, or NULL on malloc failure or if
 *                 list is NULL.
 */
t_env	**export_list_to_array(t_list *list, int *size)
{
	t_env	**export_array;
	t_list	*current;
	int		i;

	if (!list)
		return (NULL);
	*size = (int)ft_lstsize(list);
	export_array = malloc(sizeof (t_env *) * (*size + 1));
	if (!export_array)
		return (NULL);
	i = 0;
	current = list;
	while (i < *size)
	{
		export_array[i] = (t_env *)current->content;
		current = current->next;
		i++;
	}
	export_array[i] = NULL;
	return (export_array);
}

/**
 * @brief Print the environment array in export-sorted format.
 *
 * Each entry is printed like Bash's `declare -x`:
 * - With value: declare -x KEY="VALUE"
 * - Without value: declare -x KEY
 *
 * @param array Array of t_env pointers.
 * @param size Number of elements in the array.
 */
void	print_sorted_export(t_env **array, int size)
{
	int	i;

	if (!array || size <= 0)
		return ;
	i = 0;
	while (i < size)
	{
		if (array[i]->value)
			printf("declare -x %s=\"%s\"\n", array[i]->key, array[i]->value);
		else
			printf("declare -x %s\n", array[i]->key);
		i++;
	}
}
