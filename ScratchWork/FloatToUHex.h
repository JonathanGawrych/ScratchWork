#pragma once

// Warning! This fill relies on undefined behavior that is not portable!
// Do not use unless except for diagnostic / curiosity proposes only!
#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wundefined-reinterpret-cast"
#endif

template<typename FloatType>
struct type_float_to_uhex;

template<>
struct type_float_to_uhex<float>
{
	static_assert(sizeof(float) == sizeof(unsigned int));
	using Type = unsigned int;
};

template<>
struct type_float_to_uhex<double>
{
	static_assert(sizeof(double) == sizeof(unsigned long long));
	using Type = unsigned long long;
};

template<>
struct type_float_to_uhex<long double>
{
	static_assert(sizeof(long double) == sizeof(unsigned long long));
	using Type = unsigned long long;
};

template<typename FloatType>
using type_float_to_uhex_t = typename type_float_to_uhex<FloatType>::Type;

template<typename FloatType>
constexpr type_float_to_uhex_t<FloatType> float_to_uhex(FloatType f)
{
	return reinterpret_cast<type_float_to_uhex_t<FloatType>&>(f);
}

template<typename HexType>
struct type_uhex_to_float;

template<>
struct type_uhex_to_float<unsigned int>
{
	static_assert(sizeof(unsigned int) == sizeof(float));
	using Type = float;
};

template<>
struct type_uhex_to_float<unsigned long>
{
	static_assert(sizeof(unsigned long) == sizeof(float));
	using Type = float;
};

template<>
struct type_uhex_to_float<unsigned long long>
{
	static_assert(sizeof(double) == sizeof(unsigned long long));
	using Type = double;
};

template<typename HexType>
using type_uhex_to_float_t = typename type_uhex_to_float<HexType>::Type;

template<typename HexType>
constexpr type_uhex_to_float_t<HexType> uhex_to_float(HexType h)
{
	return reinterpret_cast<type_uhex_to_float_t<HexType>&>(h);
}

#ifdef __clang__
	#pragma clang diagnostic pop
#endif
