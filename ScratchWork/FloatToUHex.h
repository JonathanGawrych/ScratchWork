#pragma once

template<typename FloatType>
struct type_float_to_uhex;

template<>
struct type_float_to_uhex<float>
{
	static_assert(sizeof(float) == sizeof(int));
	using Type = unsigned int;
};

template<>
struct type_float_to_uhex<double>
{
	static_assert(sizeof(double) == sizeof(long long));
	using Type = long long;
};

template<>
struct type_float_to_uhex<long double>
{
	static_assert(sizeof(long double) == sizeof(long long));
	using Type = long long;
};

template<typename FloatType>
using type_float_to_uhex_t = typename type_float_to_uhex<FloatType>::Type;

template<typename FloatType>
constexpr type_float_to_uhex_t<FloatType> float_to_uhex(FloatType f)
{
	return reinterpret_cast<type_float_to_uhex_t<FloatType>&>(f);
}
