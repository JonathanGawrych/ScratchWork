#pragma once
#include <iostream>

#ifdef _MSC_VER
	#include <excpt.h>
#endif

template<typename Fn>
void CallBadCodeAndIgnoreDeadlyErrorsImpl(Fn&& fn)
{
	// __try __except is a MSVC concept, so if we can't use it, then don't
	#ifdef _MSC_VER
		// disable warnings for using __try __except
		#ifdef __clang__
			#pragma clang diagnostic push
			#pragma clang diagnostic ignored "-Wlanguage-extension-token"
		#endif
		__try
		{
	#endif

	fn();

	#ifdef _MSC_VER
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			std::cout << "Oops! Tried to do something really bad, ignoring" << std::endl;
		}

		#ifdef __clang__
			#pragma clang diagnostic pop
		#endif
	#endif
}

template<typename Fn>
void CallBadCodeAndIgnoreDeadlyErrors(Fn&& fn)
{
	CallBadCodeAndIgnoreDeadlyErrorsImpl([fn = std::forward<Fn>(fn)]()
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
