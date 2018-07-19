#include "ScratchPicker.h"
#include <array>

// Compiler crash on debug arm only. Ship/other platforms do not crash.

// Needs to be 0x10100 (65,792) or more
static const size_t magicSize = 0x10100;

struct Baz
{
	std::array<char, magicSize> magic;
};

// In the original code, there was more parameters (of any type) seems to reduce the needed number of `magicSize` and `bars();`
// With a const std::wstring&, a std::function<void()> (default nullptr), and other integer types, magicSize = 16 and bars = 6.
static void bar(Baz baz = Baz())
{
	(void) baz;
}

static void foo()
{
	try
	{
	}
	catch (...)
	{
		// Needs to be 13 bars or more
		// For a workaround, moving this to its own function.
		bar(); bar(); bar(); bar();
		bar(); bar(); bar(); bar();
		bar(); bar(); bar(); bar();
		bar();
	}
}

template<>
void RunScratch<ScratchWork::MSVCArmDebugCompilerCrash>()
{
	foo();
}
