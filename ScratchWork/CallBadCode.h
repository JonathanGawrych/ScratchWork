#pragma once
#include <iostream>
#include <excpt.h>

template<typename Fn>
void callBadCodeAndIgnoreDeadlyErrorsImpl(Fn&& fn)
{
	__try
	{
		fn();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		std::cout << "Oops! Tried to do something really bad, ignoring" << std::endl;
	}
}

template<typename Fn>
void callBadCodeAndIgnoreDeadlyErrors(Fn&& fn)
{
	callBadCodeAndIgnoreDeadlyErrorsImpl([fn = std::forward<Fn>(fn)]()
	{
		// This extra try can hopefully catch before relying on SEH catching
		try
		{
			fn();
		}
		catch (...)
		{
			std::cout << "Oops! Threw doing something, ignoring." << std::endl;
		}
	});
}
