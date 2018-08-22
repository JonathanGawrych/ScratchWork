#include "ScratchPicker.h"
#include <string>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <memory>

// MSVC doesn't define feature test macros because LOL
// If we are in MSVC, it's above version 1912, and it has c++17, we have extract (probably)
#if !defined(__cpp_lib_node_extract) && (!defined(__clang__) && defined(_MSC_VER) && _MSC_VER >= 1912 && _HAS_CXX17)
	#define __cpp_lib_node_extract 201606
#endif

#ifndef __cpp_lib_node_extract
	#include <type_traits>
#endif

#include "Navi.h"

template <typename AssociativeContainer>
static typename AssociativeContainer::iterator MoveNode(typename AssociativeContainer::iterator iter, AssociativeContainer& from, AssociativeContainer& to)
{
	typename AssociativeContainer::iterator iterToMove = iter++;
#ifndef __cpp_lib_node_extract
	std::cout << "extracting node " << std::distance(std::begin(from), iterToMove) << " (" << *iterToMove << ") and moving it to the other container" << std::endl;
	to.insert(from.extract(iterToMove));
#else
	if constexpr (std::is_copy_constructible_v<typename AssociativeContainer::value_type>)
	{
		std::cout << "copying value at " << std::distance(std::begin(from), iterToMove) << " (" << iterToMove->value << ") and erasing it from the original container" << std::endl;
		to.emplace(*iterToMove);
		from.erase(iterToMove);
	}
	else
	{
		std::cout << "moving value at " << std::distance(std::begin(from), iterToMove) << " (" << *iterToMove->value << ") and erasing it from the original container" << std::endl;
		std::cout << "WARNING, RELYING ON UB. See https://stackoverflow.com/q/27671303/1248889" << std::endl;
		to.emplace(const_cast<typename AssociativeContainer::value_type&&>(*iterToMove));
		from.erase(iterToMove);
	}
#endif
	return iter;
}

// Provide an operator overload for ostream to be able to consume unordered_set<int>
// TODO: make this magic to accept any container that supports range based looping
std::ostream& operator<<(std::ostream& os, const std::unordered_set<Navi<int>>& container)
{
	bool first = true;
	for (const auto& item : container)
	{
		os << (first ? "" : ", ") << item.value;
		first = false;
	}
	return os;
}

template<>
void RunScratch<ScratchWork::WhatCompiler>();

template<>
void RunScratch<ScratchWork::IsSupported>()
{
	RunScratch<ScratchWork::WhatCompiler>();

	std::unordered_set<Navi<int>> foo;
	foo.emplace(1);
	foo.emplace(2);
	foo.emplace(3);

	std::unordered_set<Navi<int>> bar;
	bar.emplace(4);
	foo.emplace(4);
	foo.emplace(6);

	std::cout << "foo: " << foo << std::endl;
	std::cout << "bar: " << bar << std::endl;

	std::cout << "Move Node" << std::endl;
	MoveNode(foo.begin(), foo, bar);

	std::cout << "foo: " << foo << std::endl;
	std::cout << "bar: " << bar << std::endl;
}

