#pragma once

enum class ScratchWork
{
	Noop,
	NumericalLimits
};

// Use template specialization to do a compile time switch on which
// piece of code should be run.
template<ScratchWork work>
void RunScratch();
