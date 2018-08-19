#include "ScratchPicker.h"

#ifdef _WIN32
	#include <cstdlib>
#endif

// Do nothing. Useful for just building.
template<>
void RunScratch<ScratchWork::Noop>() {}

constexpr ScratchWork run = ScratchWork::NaviTester;

// Explicit instantiation declaration to let the compiler know that RunScratch<run>
// is explicitly instantiated in another translation unit than main
template<>
void RunScratch<run>();

int main()
{
	RunScratch<run>();

#ifdef _WIN32
	system("pause");
#endif

	return 0;
}
