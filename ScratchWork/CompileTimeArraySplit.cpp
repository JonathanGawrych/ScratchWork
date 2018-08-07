#include "ScratchPicker.h"

#include <iostream>

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wundefined-reinterpret-cast"
	#pragma clang diagnostic ignored "-Wold-style-cast"
#endif

// Adapted from: Using the Array Template Parameter for a Return
// https://stackoverflow.com/a/36842111/1248889

// The sadist/masochistic version
// Returns the left half of an array
template <typename T, std::size_t N>
constexpr T(&leftHalf(T(&arr)[N]))[N / 2] {
	return (T(&)[N / 2])(arr);
}

// Returns the right half of the array
// Note that the above doesn't need to take an address and then dereference it
// but this transforms the lvalue arr to an rvalue, so a reference is needed
template <typename T, std::size_t N>
constexpr T(&rightHalf(T(&arr)[N]))[(N + 1) / 2] {
	return *(T(*)[(N + 1) / 2])(&arr[N / 2]);
}

// Removes the last item from the array
template <typename T, std::size_t N>
constexpr T(&oneSmaller(T(&arr)[N]))[N - 1] {
	return (T(&)[N - 1])(arr);
}

// The friendlier version, first to define an array
template <typename T, std::size_t N>
using ArrayN = T[N];

// Now define a half array. We can round up/down depending if the middle value
// of a odd array should be in the right half.
template <typename T, std::size_t N, bool roundUp = false>
using ArrayHalfN = T[(N + roundUp) / 2];

// Defining an array minus one
template <typename T, std::size_t N>
using ArrayNMM = T[N - 1];

// Pretty simple. Just cast the array to half an array.
template <typename T, std::size_t N>
constexpr ArrayHalfN<T, N>& leftHalfNicer(ArrayN<T, N>& arr) {
	return reinterpret_cast<ArrayHalfN<T, N>&>(arr);
}

// Also pretty simple, get a reference to the right half.
// Similar to the masochistic version, need to take an address
template <typename T, std::size_t N>
constexpr ArrayHalfN<T, N, true>& rightHalfNicer(ArrayN<T, N>& arr)
{
	return *reinterpret_cast<ArrayHalfN<T, N, true>*>(&arr[N / 2]);
}

// Also simple. Just casting like left half.
template <typename T, std::size_t N>
constexpr ArrayNMM<T, N>& oneSmallerNicer(ArrayN<T, N>& arr)
{
	return reinterpret_cast<ArrayNMM<T, N>&>(arr);
}

// Print the array and each half
template <typename T, std::size_t N>
void printArrays(ArrayN<T, N>& arr)
{
	// Variables could also be reprensented as
	//T (&leftHalfArr)[N / 2] = leftHalf(arr);
	//T (&rightHalfArr)[(N + 1) / 2] = rightHalf(arr);

	ArrayHalfN<T, N>& leftHalfArr = leftHalfNicer(arr);
	ArrayHalfN<T, N, true>& rightHalfArr = rightHalfNicer(arr);

	std::cout << (N % 2 == 0 ? "even:" : "odd:") << std::endl;

	std::cout << "arr" << std::endl;
	for (size_t i = 0; i < _countof(arr); i++)
	{
		std::cout << arr[i] << std::endl;
	}

	std::cout << "leftHalfArr" << std::endl;
	for (size_t i = 0; i < _countof(leftHalfArr); i++)
	{
		std::cout << leftHalfArr[i] << std::endl;
	}

	std::cout << "rightHalfArr" << std::endl;
	for (size_t i = 0; i < _countof(rightHalfArr); i++)
	{
		std::cout << rightHalfArr[i] << std::endl;
	}
}

template<>
void RunScratch<ScratchWork::CompileTimeArraySplit>()
{
	int evenArr[4] { 1, 2, 3, 4 };
	printArrays(evenArr);

	// Variables could also be reprensented as
	// int (&oddArr)[3] = oneSmaller(evenArr);

	ArrayNMM<int, 4>& oddArr = oneSmallerNicer(evenArr);

	printArrays(oddArr);
}

#ifdef __clang__
	#pragma clang diagnostic pop
#endif
