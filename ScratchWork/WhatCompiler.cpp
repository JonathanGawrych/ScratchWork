#include "ScratchPicker.h"
#include <iostream>

template<>
void RunScratch<ScratchWork::WhatCompiler>()
{
	std::cout << "What compiler?" << std::endl << std::endl;

	#if defined(__clang__)
		std::cout << "Clang/LLVM "
			<< __clang_major__ << "."
			<< __clang_minor__ << "."
			<< __clang_patchlevel__ << " - "
			<< __clang_version__ << std::endl;
	#endif

	#if defined(__ICC) || defined(__INTEL_COMPILER)
		std::cout << "Intel ICC/ICPC "
		#if defined(__ICC)
			<< __ICC << "."
		#endif
			<< __INTEL_COMPILER << " - "
			<< __INTEL_COMPILER_BUILD_DATE << std::endl;
	#endif

	#if defined(__GNUC__) || defined(__GNUG__)
		std::cout << "GNU "
		#if defined(__GNUG__)
			<< "G++ " << __GNUG__ << "."
		#else
			<< "GCC " << __GNUC__ << "."
		#endif
			<< __GNUC_MINOR__ << "."
			<< __GNUC_PATCHLEVEL__ << std::endl;
	#endif

	#if defined(__HP_cc) || defined(__HP_aCC)
		std::cout << "HP "
		#if defined(__HP_aCC)
			<< "C++ " << __HP_aCC << std::endl;
		#else
			<< "C " << __HP_cc << std::endl;
		#endif
	#endif

	#if defined(__IBMC__) || defined(__IBMCPP__)
		std::cout << " IBM XL "
		#if defined(__IBMCPP__)
			<< "C++ " << __IBMCPP__ << " - ";
		#else
			<< "C " << __IBMC__ << " - ";
		#endif
		#if defined(__xlC__)
			<< __xlC__ << " - ";
		#else
			<< __xlc__ << " - ";
		#endif
			<< __xlC_ver__ << std::endl;
	#endif

	#if defined(_MSC_VER)
		std::cout << "MSVC "
			<< _MSC_VER << " - "
			<< _MSC_FULL_VER << " - "
			<< _MSC_BUILD << std::endl;
	#endif

	#if defined(__PGI)
		std::cout << "Portland Group PGCC/PGCPP "
			<< __PGIC__ << "."
			<< __PGIC_MINOR << "."
			<< __PGIC_PATCHLEVEL__ << std::endl;
	#endif

	#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
		std::cout << "Oracle Solaris Studio "
		#if defined(__SUNPRO_CC)
			<< "C++ " << __SUNPRO_CC << std::endl;
		#else
			<< "C " << __SUNPRO_C << std::endl;
		#endif
	#endif

	#if defined(__VERSION__)
		std::cout << "VERSION " << __VERSION__ << std::endl;
	#endif

	std::cout << std::endl;
	std::cout << "Note that clang and intel also identify as GNU, so multiple compilers can output." << std::endl;
	std::cout << "If that happens, the first one is the true compiler" << std::endl << std::endl;
}
