#include "minishell.h"

/**
 * @brief Select the median of three pivot elements for quicksort.
 *
 * Uses the keys of the first, middle, and last elements in the array to
 * determine the median value for pivot selection. This helps reduce 
 * worst-case behavior in quicksort.
 *
 * @param array Array of t_env pointers.
 * @param low Index of the first element.
 * @param high Index of the last element.
 * @return int Index of the median element among low, mid, and high.
 */
int	median_of_three(t_env **array, int low, int high)
{
	int		mid;
	char	*a;
	char	*b;
	char	*c;

	mid = (low + high) / 2;
	a = array[low]->key;
	b = array[mid]->key;
	c = array[high]->key;
	if ((ft_strcmp(a, b) <= 0 && ft_strcmp(b, c) <= 0)
		|| (ft_strcmp(c, b) <= 0 && ft_strcmp(b, a) <= 0))
		return (mid);
	else if ((ft_strcmp(b, a) <= 0 && ft_strcmp(a, c) <= 0)
		|| (ft_strcmp(c, a) <= 0 && ft_strcmp(a, b) <= 0))
		return (low);
	else
		return (high);
}

/** @brief Swap two t_env pointers.
 *
 * @param a Pointer to the first t_env pointer.
 * @param b Pointer to the second t_env pointer.
 */
void	swap_env(t_env **a, t_env **b)
{
	t_env	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * @brief Partition the array for quicksort using a pivot.
 *
 * @param array Array of t_env pointers.
 * @param low Starting index.
 * @param high Ending index.
 * @return int The final index of the pivot after partitioning.
 */
int	partition(t_env **array, int low, int high)
{
	int		i;
	int		j;
	int		pivot_index;
	t_env	*pivot;

	pivot_index = median_of_three(array, low, high);
	swap_env(&array[pivot_index], &array[high]);
	pivot = array[high];
	i = low - 1;
	j = low;
	while (j <= high - 1) //dont include last node
	{
		if (ft_strcmp(array[j]->key, pivot->key) <= 0)
		{
			i++; // increment first since its -1
			swap_env(&array[i], &array[j]);
		}
		j++;
	}
	swap_env(&array[i + 1], &array[high]);
	return (i + 1);
}

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
