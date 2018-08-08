#include "ScratchPicker.h"

#include <iostream>
#include <string>

template <typename FunctionPointerType>
struct detectNoExcept;

template <typename Return, typename ... Arguments>
struct detectNoExcept<Return(*)(Arguments ...)>
{
	std::string operator()() noexcept
	{
		return "noexcept(false) specialization";
	}
};

// Using the clang LLVM for visual studio has a bug where
// this generates the same mangled name, causing an error
#if !(defined(_MSC_VER) && defined(__clang__))
template <typename Return, typename ... Arguments>
struct detectNoExcept<Return(*)(Arguments ...) noexcept>
{
	std::string operator()() noexcept
	{
		return "noexcept(true) specialization";
	}
};
#endif

template <typename Return, typename Class, typename ... Arguments>
struct detectNoExcept<Return(Class::*)(Arguments ...)>
{
	std::string operator()() noexcept
	{
		return "noexcept(false) member specialization";
	}
};

// Using the clang LLVM for visual studio has a bug where
// this generates the same mangled name, causing an error
#if !(defined(_MSC_VER) && defined(__clang__))
template <typename Return, typename Class, typename ... Arguments>
struct detectNoExcept<Return(Class::*)(Arguments ...) noexcept>
{
	std::string operator()() noexcept
	{
		return "noexcept(true) member specialization";
	}
};
#endif

void noexceptImplicitFalse() {}
void noexceptExplicitFalse() noexcept(false) {}
void noexceptImplicitTrue() noexcept {}
void noexceptExplicitTrue() noexcept(true) {}

struct Foo
{
	void noexceptImplicitFalse() {}
	void noexceptExplicitFalse() noexcept(false) {}
	void noexceptImplicitTrue() noexcept {}
	void noexceptExplicitTrue() noexcept(true) {}
};

template<>
void RunScratch<ScratchWork::SpecializeByNoexcept>()
{
	std::cout << "noexceptImplicitFalse: " << detectNoExcept<decltype(&noexceptImplicitFalse)>{}() << std::endl;
	std::cout << "noexceptExplicitFalse: " << detectNoExcept<decltype(&noexceptExplicitFalse)>{}() << std::endl;
#if !(defined(_MSC_VER) && defined(__clang__))
	std::cout << "noexceptImplicitTrue: " << detectNoExcept<decltype(&noexceptImplicitTrue)>{}() << std::endl;
	std::cout << "noexceptExplicitTrue: " << detectNoExcept<decltype(&noexceptExplicitTrue)>{}() << std::endl;
#endif

	std::cout << "noexceptImplicitFalse: " << detectNoExcept<decltype(&Foo::noexceptImplicitFalse)>{}() << std::endl;
	std::cout << "noexceptExplicitFalse: " << detectNoExcept<decltype(&Foo::noexceptExplicitFalse)>{}() << std::endl;
#if !(defined(_MSC_VER) && defined(__clang__))
	std::cout << "noexceptImplicitTrue: " << detectNoExcept<decltype(&Foo::noexceptImplicitTrue)>{}() << std::endl;
	std::cout << "noexceptExplicitTrue: " << detectNoExcept<decltype(&Foo::noexceptExplicitTrue)>{}() << std::endl;
#endif
}
