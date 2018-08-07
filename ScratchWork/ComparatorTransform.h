#pragma once
#include <utility> // std::forward
#include <functional> // std::less, std::greater, etc.
#include <type_traits> // std::declval

enum class CompartorType
{
	LessThan,
	GreaterThan,
	LessThanOrEqualTo,
	GreaterThanOrEqualTo,
	Equal,
	NotEqual
};

constexpr bool isInequality(CompartorType type)
{
	switch (type)
	{
		case CompartorType::LessThan:
		case CompartorType::GreaterThan:
		case CompartorType::LessThanOrEqualTo:
		case CompartorType::GreaterThanOrEqualTo:
			return true;

		case CompartorType::Equal:
		case CompartorType::NotEqual:
			return false;
	}
}

constexpr bool includesEqualsTo(CompartorType type)
{
	switch (type)
	{
		case CompartorType::LessThan:
		case CompartorType::GreaterThan:
		case CompartorType::NotEqual:
			return false;

		case CompartorType::LessThanOrEqualTo:
		case CompartorType::GreaterThanOrEqualTo:
		case CompartorType::Equal:
			return true;
	}
}

// Return the type you get if you were to a binary not on the result of the comparison
constexpr CompartorType opposite(CompartorType type)
{
	switch (type)
	{
		case CompartorType::LessThan: return CompartorType::GreaterThanOrEqualTo;
		case CompartorType::GreaterThanOrEqualTo: return CompartorType::LessThan;

		case CompartorType::GreaterThan: return CompartorType::LessThanOrEqualTo;
		case CompartorType::LessThanOrEqualTo: return CompartorType::GreaterThan;

		case CompartorType::Equal: return CompartorType::NotEqual;
		case CompartorType::NotEqual: return CompartorType::Equal;
	}
}

template<CompartorType type>
constexpr auto getStdComparison()
{
	// need to if else since switch constexpr doesn't exist
	if constexpr (type == CompartorType::LessThan) return std::less<>();
	else if constexpr (type == CompartorType::GreaterThan) return std::greater<>();
	else if constexpr (type == CompartorType::LessThanOrEqualTo) return std::less_equal<>();
	else if constexpr (type == CompartorType::GreaterThanOrEqualTo) return std::greater_equal<>();
	else if constexpr (type == CompartorType::Equal) return std::equal_to<>();
	else if constexpr (type == CompartorType::NotEqual) return std::not_equal_to<>();
}

template<CompartorType To, CompartorType From, typename Comparator>
struct ConvertedComparator
{
	static_assert(!isInequality(To) || isInequality(From), "If going from == or !=, it cannot be converted to <, <=, >, or >=");

	ConvertedComparator(Comparator comparator) : m_comparator(comparator) { }

	template<class _Ty1, class _Ty2>
	constexpr auto operator()(_Ty1&& a, _Ty2&& b) const -> decltype(std::declval<Comparator>()(std::forward<_Ty1>(a), std::forward<_Ty2>(b)))
	{
		if constexpr (To == From || To == opposite(From))
		{
			constexpr bool invertResult = To == opposite(From);

			return m_comparator(std::forward<_Ty1>(a), std::forward<_Ty2>(b)) != invertResult;
		}
		else
		{
			auto forwards = m_comparator(a, b);
			auto backwards = m_comparator(std::forward<_Ty2>(b), std::forward<_Ty1>(a));

			constexpr bool invertForwards = includesEqualsTo(From);
			constexpr bool invertBackwards = isInequality(To) != includesEqualsTo(From);
			constexpr bool invertResult = isInequality(To) != includesEqualsTo(To);

			return ((forwards != invertForwards) || (backwards != invertBackwards)) != invertResult;
		}
	}

	Comparator m_comparator;
};

// Makes the comparitor from a custom comparator
template<CompartorType To, CompartorType From, typename Comparator>
ConvertedComparator<To, From, Comparator> makeConvertedComparator(Comparator&& comparator)
{
	return { std::forward<Comparator>(comparator) };
}

// Makes the comparitor from the std comparitors
// This isn't very useful since you knowthe std you want
// Moreso just used to test ConvertedComparator
template<CompartorType To, CompartorType From>
ConvertedComparator<To, From, decltype(getStdComparison<From>())> makeConvertedComparator()
{
	return { getStdComparison<From>() };
}
