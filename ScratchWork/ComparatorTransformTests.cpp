#include "ScratchPicker.h"

#include "ComparatorTransform.h"

#include <functional> // std::less, std::greater
#include <iostream> // std::cout
#include <ostream> // std::endl
#include <ios> // std::streamsize, std::boolalpha
#include <iomanip> // std::setw
#include <string> // std::string
#include <cstring> // std::strlen

static const std::streamsize inputWidth = 6;
static const std::streamsize expectedWidth = 20;
static const std::streamsize actualWidth = 20;
static const std::streamsize matchesWidth = 9;

template<typename ExpectedComparison, typename ActualComparison>
static void TestTruthTable(ExpectedComparison expected, ActualComparison actual, const std::string& expectedName, const std::string& actualName)
{
	std::cout                   << std::setw(inputWidth) << "Input"  << std::setw(expectedWidth) << expectedName   << std::setw(actualWidth) << actualName   << std::setw(matchesWidth) << "Matches"                      << std::endl;
	std::cout << std::boolalpha << std::setw(inputWidth) << "1, 1" << std::setw(expectedWidth) << expected(1, 1) << std::setw(actualWidth) << actual(1, 1) << std::setw(matchesWidth) << (expected(1, 1) == actual(1, 1)) << std::endl;
	std::cout << std::boolalpha << std::setw(inputWidth) << "1, 2" << std::setw(expectedWidth) << expected(1, 2) << std::setw(actualWidth) << actual(1, 2) << std::setw(matchesWidth) << (expected(1, 2) == actual(1, 2)) << std::endl;
	std::cout << std::boolalpha << std::setw(inputWidth) << "2, 1" << std::setw(expectedWidth) << expected(2, 1) << std::setw(actualWidth) << actual(2, 1) << std::setw(matchesWidth) << (expected(2, 1) == actual(2, 1)) << std::endl;
	std::cout << std::endl;

	if ((expected(1, 1) != actual(1, 1)) ||
		(expected(1, 2) != actual(1, 2)) ||
		(expected(2, 1) != actual(2, 1)))
		throw "Not matching!";
}

template<CompartorType To>
static void TestConvertAllTo()
{
	const char* stdRawName = typeid(getStdComparison<To>()).name();
	std::string stdName { stdRawName + 7 /*remove "struct "*/, std::strlen(stdRawName) - 13 /*remove "<void>"*/};

	TestTruthTable(getStdComparison<To>(), makeConvertedComparator<To, CompartorType::LessThan>(), stdName, "Convert From <");
	TestTruthTable(getStdComparison<To>(), makeConvertedComparator<To, CompartorType::GreaterThan>(), stdName, "Convert From >");
	TestTruthTable(getStdComparison<To>(), makeConvertedComparator<To, CompartorType::LessThanOrEqualTo>(), stdName, "Convert From <=");
	TestTruthTable(getStdComparison<To>(), makeConvertedComparator<To, CompartorType::GreaterThanOrEqualTo>(), stdName, "Convert From >=");

	if constexpr (!isInequality(To))
	{
		TestTruthTable(getStdComparison<To>(), makeConvertedComparator<To, CompartorType::Equal>(), stdName, "Convert From ==");
		TestTruthTable(getStdComparison<To>(), makeConvertedComparator<To, CompartorType::NotEqual>(), stdName, "Convert From !=");
	}
}

template<>
void RunScratch<ScratchWork::ComparatorTransform>()
{
	TestConvertAllTo<CompartorType::LessThan>();
	TestConvertAllTo<CompartorType::GreaterThan>();
	TestConvertAllTo<CompartorType::LessThanOrEqualTo>();
	TestConvertAllTo<CompartorType::GreaterThanOrEqualTo>();
	TestConvertAllTo<CompartorType::Equal>();
	TestConvertAllTo<CompartorType::NotEqual>();
}