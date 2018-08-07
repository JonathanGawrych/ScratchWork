#include "ScratchPicker.h"

#include <iostream> // std::cout, std::endl
#include <vector> // std::vector
#include <algorithm> // std::is_sorted_until, std::upper_bound, std::iter_swap std::reverse, std::next_permutation
#include <functional> // std::less
#include <type_traits> // std::is_same
#include <iterator> // std::reverse_iterator, std::make_reverse_iterator, std::iterator_traits, std::random_access_iterator_tag

// Code based off of regular next_permutation code found here: https://stackoverflow.com/a/32695398/1248889
// Modification based off of permutation/combination question here: https://stackoverflow.com/a/51292710/1248889
template <typename Iterator, typename Comparator = std::less<>>
static bool next_permutation_combination(Iterator begin, Iterator end, typename Iterator::difference_type combo_size, Comparator less_than = std::less<>())
{
	static_assert(std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::bidirectional_iterator_tag>::value);

	// Look at the range backwards
	std::reverse_iterator<Iterator> rbegin = std::make_reverse_iterator(end);
	std::reverse_iterator<Iterator> rend = std::make_reverse_iterator(begin);
	
	// Here is the modification
	// By reversing the part we don't care about, we always "skip" the permutations in that range
	std::reverse(rbegin, rend - combo_size);

	// Find the next unsorted (1 4 5 3 2 => 2, remember to look from behind, 2 < 3, 3 < 5, 5 > 4)
	std::reverse_iterator<Iterator> next_unsorted = std::is_sorted_until(rbegin, rend, less_than);
	
	// If everything is sorted backwards (5 4 3 2 1) then we are done.
	if (next_unsorted == rend)
	{
		// Reverse back to 1 2 3 4 5 as convenience (and matches next_permutation)
		std::reverse(rbegin, next_unsorted);
		return false;
	}

	// Find the first value that's greater than next_unsorted (1 4 5 3 2 => 5, remember to look from behind, 4 > 2, 4 > 3, 4 < 5)
	std::reverse_iterator<Iterator> next_permutation = std::upper_bound(rbegin, next_unsorted, *next_unsorted, less_than);

	// Swap the two (1 4 5 3 2 => 1 5 4 3 2)
	std::iter_swap(next_unsorted, next_permutation);

	// Reverse the end so that its ascending again (1 5 2 3 4)
	std::reverse(rbegin, next_unsorted);

	return true;
}

#include "ComparatorTransform.h"

// Using the above logic and some work of my own, writing next_combination
template <typename Iterator, typename Comparator = std::less<>>
static bool next_combination(Iterator begin, Iterator end, typename Iterator::difference_type combo_size, Comparator less_than = std::less<>())
{
	static_assert(std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::bidirectional_iterator_tag>::value);

	// Look at the range backwards
	std::reverse_iterator<Iterator> rbegin = std::make_reverse_iterator(end);
	std::reverse_iterator<Iterator> rend = std::make_reverse_iterator(begin);

	// Find where the range is not sorted backwards. This skips lower values that have already been in combinations
	std::reverse_iterator<Iterator> combo_pivot = rend - combo_size;
	std::reverse_iterator<Iterator> skip_perm_begin = std::is_sorted_until(rbegin, combo_pivot, less_than) - 1;

	// By reversing this part, we always "skip" the permutations in this range
	// This is similar to the modification in next_permutation_combination, but skips lower values
	std::reverse(skip_perm_begin, combo_pivot);

	// Find the next item to swap. This is a little complex, but we are searching backwards from the combo_pivot for the first number that's strictly less than the combo_pivot
	// Here ConvertedComparator will transform the less than into a greater than. This is mostly equivalent to [&](auto&& a, auto&& b){ return !less_than(a, b) && less_than(b, a) };
	// We are using greater than because we next_unsorted needs ascending values, and this is a descending range.
	// See https://stackoverflow.com/a/50225224/1248889
	std::reverse_iterator<Iterator> next_unsorted = std::upper_bound(combo_pivot, rend, *(combo_pivot - 1), makeConvertedComparator<CompartorType::GreaterThan, CompartorType::LessThan>(less_than));

	// If there is not next_unsorted, then we are finished
	if (next_unsorted == rend)
	{
		// Reverse back to 1 2 3 4 5 as convenience (and matches next_permutation)
		std::reverse(combo_pivot, next_unsorted);
		std::reverse(rbegin, rend);
		return false;
	}

	// Search backwards for the first value that's strictly greater than next_unsorted
	std::reverse_iterator<Iterator> next_permutation = std::upper_bound(rbegin, next_unsorted, *next_unsorted, less_than);

	// Swap the two
	std::iter_swap(next_unsorted, next_permutation);

	// At this point the range between the combo_pivot and next_unsorted is ordered ascending, and between the beginning (sans-ignored values) and the combo_pivot is descending.
	// We want to order this all ascending, but we can't just swap the end because there will be a jump between the combo_pivot, so swap this so its descending.
	std::reverse(combo_pivot, next_unsorted);

	// Find the first value that's not ignored due to being lower than the next_permutation (remember at this point the iterators are swapped)
	std::reverse_iterator<Iterator> first_unignored = std::lower_bound(next_permutation, next_unsorted, *next_unsorted, less_than);

	// Now that the previous reverse has lined everything up, everything is descending. Make it ascending.
	std::reverse(first_unignored, next_unsorted);

	return true;
}

static void uniquePermutations()
{
	std::cout << "Permutations of unique items:" << std::endl;
	std::vector<int> items = { 1, 2, 3 };
	do
	{
		std::cout << items[0] << items[1] << items[2] << std::endl;

	} while (std::next_permutation(items.begin(), items.end()));

	std::cout << std::endl;
}

static void nonuniquePermutations()
{
	std::cout << "Permutations of nonunique items:" << std::endl;
	std::vector<int> items = { 1, 1, 2, 2 };
	do
	{
		std::cout << items[0] << items[1] << items[2] << items[3] << std::endl;

	} while (std::next_permutation(items.begin(), items.end()));

	std::cout << std::endl;
}

static void uniqueCombinations()
{
	std::vector<int> items = { 1, 2, 3, 4, 5 };
	std::cout << "Combinations of size-3 Combinations of unique items (" << items[0] << ", " << items[1] << ", " << items[2] << ", " << items[3] << ", " << items[4] << "):" << std::endl;
	do
	{
		std::cout << items[0] << items[1] << items[2] << std::endl;

	} while (next_combination(items.begin(), items.end(), 3));

	std::cout << std::endl;
}

static void nonuniqueCombinations()
{
	std::vector<int> items = { 1, 2, 2, 3, 3 };
	std::cout << "Combinations of size-3 Combinations of nonunique items (" << items[0] << ", " << items[1] << ", " << items[2] << ", " << items[3] << ", " << items[4] << "):" << std::endl;
	do
	{
		std::cout << items[0] << items[1] << items[2] << std::endl;

	} while (next_combination(items.begin(), items.end(), 3));

	std::cout << std::endl;
}

static void uniquePermutationsCombinations()
{
	std::vector<int> items = { 1, 2, 3, 4 };
	std::cout << "Permutations of size-2 Combinations of unique items (" << items[0] << ", " << items[1] << ", " << items[2] << "):" << std::endl;
	do
	{
		std::cout << items[0] << items[1] << std::endl;

	} while (next_permutation_combination(items.begin(), items.end(), 2));

	std::cout << std::endl;
}

static void nonuniquePermutationsCombinations()
{
	std::vector<int> items = { 1, 2, 2, 3 };
	std::cout << "Permutations of size-3 Combinations of nonunique items (" << items[0] << ", " << items[1] << ", " << items[2] << ", " << items[3] << "):" << std::endl;
	do
	{
		std::cout << items[0] << items[1] << items[2] << std::endl;

	} while (next_permutation_combination(items.begin(), items.end(), 3));

	std::cout << std::endl;
}

template<>
void RunScratch<ScratchWork::PermuteCombinations>()
{
	uniquePermutations();
	nonuniquePermutations();

	uniqueCombinations();
	nonuniqueCombinations();

	uniquePermutationsCombinations();
	nonuniquePermutationsCombinations();
}
