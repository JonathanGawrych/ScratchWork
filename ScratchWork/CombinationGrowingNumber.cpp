#include "ScratchPicker.h"
#include <iostream>
#include <vector>
#include <chrono>

// Will call the lambda with all combinations of 0 to M of N numbers minus
// all combinations of 0 to M - 1 of N numbers.
template<typename Lambda>
void CombinationGrowing(unsigned char N, unsigned char M, Lambda&& lambda)
{
	if (N == 0)
		return;

	if (M == 0)
	{
		lambda(std::vector<unsigned char>(N));
		return;
	}

	// Lock each value once so that we aren't redoing lower order combinations
	for (std::size_t lockedVar = 0; lockedVar < N; lockedVar++)
	{
		std::vector<unsigned char> values(N);

		// The value at the locked var is maxed
		values[lockedVar] = M;

		// While carry doesn't flow off the end of values
		std::size_t carry = 0;
		while (carry < N)
		{
			// Run the lambda
			lambda(values);

			// Find which position to increment
			// Search while carry is less than N, and the value at the carry is greater than equal to the max value.
			// Depending on if this is on the left or right hand side of the lockedVar, allow the number to grow to the max, preventing duplicates.
			for (carry = 0; carry < N && values[carry] >= M - (carry > lockedVar); carry++)
			{
				// But skip our locked variable
				if (carry == lockedVar)
					continue;

				// Reset the value at carry as if it was incremented, then wrapped back to zero and we carry the one.
				values[carry] = 0;
			}

			// If carry didn't flow off the end, finally increment the value at the right spot
			if (carry < N)
				values[carry]++;
		}
	}
}

template<>
void RunScratch<ScratchWork::CombinationGrowingNumber>()
{
	for (unsigned char M = 0; M <= 3; M++)
	{
		std::cout << "Combo " << +M << ":" << std::endl;
		size_t numCombos = 0;
		CombinationGrowing(3, M, [&](const std::vector<unsigned char>& arr)
		{
			numCombos++;
			for (unsigned char c : arr)
			{
				std::cout << +c;
			}
			std:: cout << std::endl;
		});
		std::cout << "Total: " << numCombos << std::endl;
		std::cout << std::endl;
	}

	for (volatile unsigned long long warmup = 0; warmup < (1ULL << 30); warmup++);

	for (unsigned char rounds = 0; rounds <= 20; rounds++)
	{
		unsigned long long num = 0;
		std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
		for (unsigned char M = 0; M <= rounds; M++)
		{
			CombinationGrowing(3, M, [&](const auto&)
			{
				num++;
			});
		}
		std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
		std::chrono::steady_clock::time_point::duration durationCombinationsGrowing = end - start;

		num = 0;
		start = std::chrono::high_resolution_clock::now();
		for (unsigned char M = 0; M <= rounds; M++)
		{
			for (unsigned char i = 0; i <= M; i++)
			{
				for (unsigned char j = 0; j <= M; j++)
				{
					for (unsigned char k = 0; k <= M; k++)
					{
						if (i != M && j != M && k != M)
							continue;

						num++;
					}
				}
			}
		}
		end = std::chrono::high_resolution_clock::now();
		std::chrono::steady_clock::time_point::duration durationNaive = end - start;
		std::cout << +rounds << "\t" << durationCombinationsGrowing.count() << "\t" << durationNaive.count() << std::endl;
	}
}
