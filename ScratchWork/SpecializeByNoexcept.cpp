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

template <typename Return, typename ... Arguments>
struct detectNoExcept<Return(*)(Arguments ...) noexcept>
{
	std::string operator()() noexcept
	{
		return "noexcept(true) specialization";
	}
};

void noexceptImplicitFalse() {}
void noexceptExplicitFalse() noexcept(false) {}
void noexceptImplicitTrue() noexcept {}
void noexceptExplicitTrue() noexcept(true) {}

template<>
void RunScratch<ScratchWork::SpecializeByNoexcept>()
{
	std::cout << "noexceptImplicitFalse: " << detectNoExcept<decltype(&noexceptImplicitFalse)>{}() << std::endl;
	std::cout << "noexceptExplicitFalse: " << detectNoExcept<decltype(&noexceptExplicitFalse)>{}() << std::endl;
	std::cout << "noexceptImplicitTrue: " << detectNoExcept<decltype(&noexceptImplicitTrue)>{}() << std::endl;
	std::cout << "noexceptExplicitTrue: " << detectNoExcept<decltype(&noexceptExplicitTrue)>{}() << std::endl;
}
