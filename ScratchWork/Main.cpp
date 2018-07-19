#include "ScratchPicker.h"

#ifdef _WIN32
#include <cstdlib>
#endif

// Do nothing. Useful for just building.
template<>
void RunScratch<ScratchWork::Noop>() {}

int main()
{
	RunScratch<ScratchWork::NumericalLimits>();

#ifdef _WIN32
	system("pause");
#endif

	return 0;
}