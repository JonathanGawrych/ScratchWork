#include "ScratchPicker.h"
#include <limits>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <string>

#include "FloatToUHex.h"

static const std::streamsize columnNameWidth = 20;
static const std::streamsize columnSizeWidth = 6;
static const std::streamsize intColumnLimitWidth = 23;
static const std::streamsize floatColumnLimitWidth = 18;

template<typename numeric_primative>
static void printType(std::string name = "")
{
	std::cout << std::setw(columnNameWidth) << (name.empty() ? typeid(numeric_primative).name() : name);
	std::cout << std::setw(columnSizeWidth) << sizeof(numeric_primative);

	constexpr const std::streamsize columnLimit = std::numeric_limits<numeric_primative>::is_integer ? intColumnLimitWidth : floatColumnLimitWidth;

	if constexpr (std::numeric_limits<numeric_primative>::is_bounded)
	{
		std::cout << std::setw(columnLimit) << +std::numeric_limits<numeric_primative>::max();
		std::cout << std::setw(columnLimit) << +std::numeric_limits<numeric_primative>::lowest();
	}
	if constexpr (std::numeric_limits<numeric_primative>::is_bounded || !std::numeric_limits<numeric_primative>::is_signed)
	{
		std::cout << std::setw(columnLimit) << +std::numeric_limits<numeric_primative>::min();
	}
	if constexpr (!std::numeric_limits<numeric_primative>::is_integer)
	{
		std::cout << std::setw(columnLimit) << +std::numeric_limits<numeric_primative>::epsilon();
		std::cout << std::setw(columnLimit) << +std::numeric_limits<numeric_primative>::round_error();
	}
	if constexpr (std::numeric_limits<numeric_primative>::has_infinity)
	{
		std::cout << std::setw(columnLimit) << std::hex << +float_to_uhex(std::numeric_limits<numeric_primative>::infinity());
	}
	if constexpr (std::numeric_limits<numeric_primative>::has_quiet_NaN)
	{
		std::cout << std::setw(columnLimit) << std::hex << +float_to_uhex(std::numeric_limits<numeric_primative>::quiet_NaN());
	}
	if constexpr (std::numeric_limits<numeric_primative>::has_signaling_NaN)
	{
		std::cout << std::setw(columnLimit) << std::hex << +float_to_uhex(std::numeric_limits<numeric_primative>::signaling_NaN());
	}
	if constexpr (std::numeric_limits<numeric_primative>::has_denorm != std::denorm_absent)
	{
		std::cout << std::setw(columnLimit) << +std::numeric_limits<numeric_primative>::denorm_min();
	}
	std::cout << std::endl;
}

template<>
void RunScratch<ScratchWork::NumericalLimits>()
{
	std::cout << std::setw(columnNameWidth) << "Int Type";
	std::cout << std::setw(columnSizeWidth) << "Size";
	std::cout << std::setw(intColumnLimitWidth) << "Max";
	std::cout << std::setw(intColumnLimitWidth) << "Lowest";
	std::cout << std::setw(intColumnLimitWidth) << "Min";
	std::cout << std::endl;

	printType<bool>();
	printType<char>();
	printType<signed char>();
	printType<unsigned char>();
	printType<wchar_t>();
	printType<char16_t>();
	printType<char32_t>();
	printType<short>();
	printType<unsigned short>();
	printType<int>();
	printType<unsigned int>();
	printType<long>();
	printType<unsigned long>();
	printType<long long>("long long");
	printType<unsigned long long>("unsigned long long");
	printType<std::size_t>("size_t");

	std::cout << std::endl;

	std::cout << std::setw(columnNameWidth) << "Float Type";
	std::cout << std::setw(columnSizeWidth) << "Size";
	std::cout << std::setw(floatColumnLimitWidth) << "Max";
	std::cout << std::setw(floatColumnLimitWidth) << "Lowest";
	std::cout << std::setw(floatColumnLimitWidth) << "Min";
	std::cout << std::setw(floatColumnLimitWidth) << "Epsilon";
	std::cout << std::setw(floatColumnLimitWidth) << "Round Error";
	std::cout << std::setw(floatColumnLimitWidth) << "Infinity";
	std::cout << std::setw(floatColumnLimitWidth) << "Quiet NaN";
	std::cout << std::setw(floatColumnLimitWidth) << "Signaling NaN";
	std::cout << std::setw(floatColumnLimitWidth) << "Denormal Min";
	std::cout << std::endl;
	printType<float>();
	printType<double>();
	printType<long double>();

	std::cout << std::endl;
}
