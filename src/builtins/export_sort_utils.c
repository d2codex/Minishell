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
