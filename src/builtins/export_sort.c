#include "minishell.h"

/**
 * @brief Recursively sorts an array of t_env pointers by key using quicksort.
 *
 * Quicksort picks a pivot, partitions the array so keys <= pivot come first,
 * then sorts the left and right partitions recursively. Small arrays (<=16)
 * are skipped here and handled by insertion sort later for efficiency.
 *
 * @param array Array of t_env pointers to sort.
 * @param low   Starting index of the subarray.
 * @param high  Ending index of the subarray.
 */
void	quicksort(t_env **array, int low, int high)
{
	int	pivot_index;

	if (!array)
		return ;
	if ((high - low + 1) <= 16)
		return ;
	if (low < high)
	{
		pivot_index = partition(array, low, high);
		quicksort(array, low, pivot_index - 1);
		quicksort(array, pivot_index + 1, high);
	}
}

/**
 * @brief Sorts a small array of t_env pointers by key using insertion sort.
 *
 * Insertion sort is efficient for small arrays and works by taking each
 * element and inserting it into its correct position among the previously
 * sorted elements.
 *
 * @param array Array of t_env pointers to sort.
 * @param size  Number of elements in the array.
 */
void	insertion_sort_env(t_env **array, int size)
{
	int		i;
	int		j;
	t_env	*key;

	i = 1;
	while (i < size)
	{
		key = array[i];
		j = i - 1;
		while (j >= 0 && ft_strcmp(array[j]->key, key->key) > 0)
		{
			array[j + 1] = array[j];
			j--;
		}
		array[j + 1] = key;
		i++;
	}
}

/**
 * @brief Sorts an array of t_env pointers by key in ascending order.
 *
 * Chooses the sorting algorithm based on array size:
 * - Does nothing if the array is NULL or has 1 or fewer elements.
 * - For 2 elements, performs a simple comparison and swap.
 * - For small arrays (<=16), uses insertion sort for efficiency.
 * - For larger arrays, uses quicksort followed by insertion sort
 *   to finish any small unsorted partitions (hybrid approach).
 *
 * @param array Array of t_env pointers to sort.
 * @param size  Number of elements in the array.
 */
void	sort_export_array(t_env **array, int size)
{
	if (!array)
		return ;
	if (size == 2)
	{
		if (ft_strcmp(array[0]->key, array[1]->key) > 0)
			swap_env(&array[0], &array[1]);
	}
	else if (size <= 16)
		insertion_sort_env(array, size);
	else
	{
		quicksort(array, 0, size - 1);
		insertion_sort_env(array, size);
	}
}
