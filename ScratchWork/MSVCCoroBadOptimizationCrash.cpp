#include "ScratchPicker.h"
#include "CallBadCode.h"

#include <experimental/coroutine>
#include <future>
#include <iostream>
#include <cstdlib>

// Cancelling a coroutine causes a garbage pointer to be passed to delete
// Runtime crash in release only when on MSVC 14.13.26128 or lower

#if defined(__clang__) && defined(_MSVC)

struct Foo
{
};

// Disable warning unreachable
#pragma warning(push)
#pragma warning(disable: 4702)
std::future<Foo> OpenAsync()
{
	throw 0x80004005;

	co_return Foo {};
}
#pragma warning(pop)

std::future<bool> GetFooAsync()
{
	// throwing here does not cause a crash

	Foo myFoo = co_await OpenAsync();

	return false;
}

template<>
void RunScratch<ScratchWork::MSVCCoroBadOptimizationCrash>()
{
	CallBadCodeAndIgnoreDeadlyErrors([]()
	{
		try
		{
			GetFooAsync().get();
		}
		catch (...)
		{
			std::cout << "we threw as expected!" << std::endl;
		}
	});
}

#else
template<>
void RunScratch<ScratchWork::MSVCCoroBadOptimizationCrash>()
{
	std::cout << "Clang for visual studio doesn't support coroutines" << std::endl;
}
#endif

// By disabling this optimization *after* this file, the crash doesn’t happen
//#pragma optimize("g", off)
