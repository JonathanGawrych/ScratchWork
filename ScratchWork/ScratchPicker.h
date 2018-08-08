#pragma once

enum class ScratchWork
{
	Noop,
	NumericalLimits,
	GenTrees,
	MSVCArmDebugCompilerCrash,
	CoincidentalLifetimeManagement,
	CompileTimeArraySplit,
	CombinationGrowingNumber,
	ComparatorTransform,
	PermuteCombinations,
	SpecializeByNoexcept,
	WhatCompiler
};

// Use template specialization to do a compile time switch on which
// piece of code should be run.
template<ScratchWork work>
void RunScratch();

constexpr ScratchWork run = ScratchWork::SpecializeByNoexcept;

// Explicit instantiation declaration to let the compiler know that RunScratch<run>
// is explicitly instantiated in another translation unit than main
template<>
void RunScratch<run>();
