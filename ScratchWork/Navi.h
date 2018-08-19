#pragma once
#include <iostream> // std::cout, std::endl
#include <utility> // std::move
#include <type_traits>

// A very noisy fairy that tells you when any of the methods are called
// Nice to have to detected copies as opposed to references or moves
// Note that this fairy tries to copy the type it encapsulates  
template<typename T,
	bool = std::is_move_constructible_v<T>,
	bool = std::is_move_assignable_v<T>,
	bool = std::is_copy_constructible_v<T>,
	bool = std::is_copy_assignable_v<T>,
	bool = std::is_constructible_v<T, T>,
	bool = std::is_default_constructible_v<T>,
	bool = std::is_destructible_v<T>>
struct Navi;

template<typename T>
struct Navi<T, true /*moveConstruct*/, true /*moveAssign*/, true /*copyConstruct*/, true /*copyAssign*/, true /*valueConstruct*/, true /*defaultConstruct*/, true /*destructable*/>
{
	T value;
	bool operatorCastVerbose = true;

	explicit operator T&() & noexcept
	{
		if (operatorCastVerbose)
			std::cout << "Navi T non-const lvalue cast operator (" << this << "(" << value << "))" << std::endl;
		return value;
	}

	explicit operator const T&() const& noexcept
	{
		if (operatorCastVerbose)
			std::cout << "Navi T const lvalue cast operator (" << this << "(" << value << "))" << std::endl;
		return value;
	}

	explicit operator const T && () && noexcept
	{
		if (operatorCastVerbose)
			std::cout << "Navi T non-const rvalue cast operator (" << this << "(" << value << "))" << std::endl;
		return std::move(value);
	}

	bool operator==(const Navi& other) const noexcept
	{
		return value == other.value;
	}

	Navi(Navi&& other) noexcept : value(std::move(other.value))
	{
		std::cout << "Navi move constructor (" << this << " moving " << &other << "(" << value << "))" << std::endl;
	}

	Navi& operator=(Navi&& other) noexcept
	{
		value = std::move(other.value);
		std::cout << "Navi move assignment operator (" << this << " moving " << &other << "(" << value << "))" << std::endl;
		return *this;
	}

	Navi(const Navi& other) noexcept : value(other.value)
	{
		std::cout << "Navi copy constructor (" << this << " copying " << &other << "(" << other.value << "))" << std::endl;
	}

	Navi& operator=(const Navi& other) noexcept
	{
		value = other.value;
		std::cout << "Navi copy assignment operator (" << this << " copying " << &other << "(" << other.value << "))" << std::endl;
		return *this;
	}

	Navi(T v) noexcept : value(v)
	{
		std::cout << "Navi constructor with value (" << this << "(" << value << "))" << std::endl;
	}

	Navi() noexcept
	{
		std::cout << "Navi default constructor (" << this << ")" << std::endl;
	}

	~Navi() noexcept
	{
		std::cout << "Navi destructor (" << this << "(" << value << "))" << std::endl;
	}
};

// Non-destructable
template<typename T>
struct Navi<T, true /*moveConstruct*/, true /*moveAssign*/, true /*copyConstruct*/, true /*copyAssign*/, true /*valueConstruct*/, true /*defaultConstruct*/, false /*destructable*/>
     : Navi<T, true /*moveConstruct*/, true /*moveAssign*/, true /*copyConstruct*/, true /*copyAssign*/, true /*valueConstruct*/, true /*defaultConstruct*/,  true /*destructable*/>
{
	~Navi() = delete;

	// Inherit
	using Navi<T, true /*moveConstruct*/, true /*moveAssign*/, true /*copyConstruct*/, true /*copyAssign*/, true /*valueConstruct*/, true /*defaultConstruct*/, true /*destructable*/>::Navi;
	Navi() = default;
	Navi& operator=(const Navi& other) = default;
	Navi(const Navi& other) = default;
	Navi(Navi&& other) = default;
	Navi& operator=(Navi&& other) = default;
};

// Disable warning C4624: destructor was implicitly defined as deleted if destructable is false
#pragma warning( push )
#pragma warning( disable : 4624)

// Non-default-constructable
template<typename T, bool destructable>
struct Navi<T, true /*moveConstruct*/, true /*moveAssign*/, true /*copyConstruct*/, true /*copyAssign*/, true /*valueConstruct*/, false /*defaultConstruct*/, destructable>
     : Navi<T, true /*moveConstruct*/, true /*moveAssign*/, true /*copyConstruct*/, true /*copyAssign*/, true /*valueConstruct*/,  true /*defaultConstruct*/, destructable>
{
	Navi() = delete;

	// Inherit
	~Navi() = default;
	using Navi<T, true /*moveConstruct*/, true /*moveAssign*/, true /*copyConstruct*/, true /*copyAssign*/, true /*valueConstruct*/, true /*defaultConstruct*/, destructable>::Navi;
	Navi& operator=(const Navi& other) = default;
	Navi(const Navi& other) = default;
	Navi(Navi&& other) = default;
	Navi& operator=(Navi&& other) = default;
};

// Non-value-constructable
template<typename T, bool defaultConstruct, bool destructable>
struct Navi<T, true /*moveConstruct*/, true /*moveAssign*/, true /*copyConstruct*/, true /*copyAssign*/, false /*valueConstruct*/, defaultConstruct, destructable>
     : Navi<T, true /*moveConstruct*/, true /*moveAssign*/, true /*copyConstruct*/, true /*copyAssign*/,  true /*valueConstruct*/, defaultConstruct, destructable>
{
	Navi(T v) = delete;

	// Inherit
	~Navi() = default;
	Navi() = default;
	using Navi<T, true /*moveConstruct*/, true /*moveAssign*/, true /*copyConstruct*/, true /*copyAssign*/, true /*valueConstruct*/, defaultConstruct, destructable>::Navi;
	Navi& operator=(const Navi& other) = default;
	Navi(const Navi& other) = default;
	Navi(Navi&& other) = default;
	Navi& operator=(Navi&& other) = default;
};

// Non-copy-assignable
template<typename T, bool valueConstruct, bool defaultConstruct, bool destructable>
struct Navi<T, true /*moveConstruct*/, true /*moveAssign*/, true /*copyConstruct*/, false /*copyAssign*/, valueConstruct, defaultConstruct, destructable>
     : Navi<T, true /*moveConstruct*/, true /*moveAssign*/, true /*copyConstruct*/,  true /*copyAssign*/, valueConstruct, defaultConstruct, destructable>
{
	Navi& operator=(const Navi& other) = delete;

	// Inherit
	~Navi() = default;
	Navi() = default;
	using Navi<T, true /*moveConstruct*/, true /*moveAssign*/, true /*copyConstruct*/, true /*copyAssign*/, valueConstruct, defaultConstruct, destructable>::Navi;
	Navi(const Navi& other) = default;
	Navi(Navi&& other) = default;
	Navi& operator=(Navi&& other) = default;
};

// Non-copy-constructable
template<typename T, bool copyAssign, bool valueConstruct, bool defaultConstruct, bool destructable>
struct Navi<T, true /*moveConstruct*/, true /*moveAssign*/, false /*copyConstruct*/, copyAssign, valueConstruct, defaultConstruct, destructable>
     : Navi<T, true /*moveConstruct*/, true /*moveAssign*/,  true /*copyConstruct*/, copyAssign, valueConstruct, defaultConstruct, destructable>
{
	Navi(const Navi& other) = delete;

	// Inherit
	~Navi() = default;
	Navi() = default;
	using Navi<T, true /*moveConstruct*/, true /*moveAssign*/, true /*copyConstruct*/, copyAssign, valueConstruct, defaultConstruct, destructable>::Navi;
	Navi& operator=(const Navi& other) = default;
	Navi(Navi&& other) = default;
	Navi& operator=(Navi&& other) = default;
};

// Non-move-assignable
template<typename T, bool copyConstruct, bool copyAssign, bool valueConstruct, bool defaultConstruct, bool destructable>
struct Navi<T, true /*moveConstruct*/, false /*moveAssign*/, copyConstruct, copyAssign, valueConstruct, defaultConstruct, destructable>
     : Navi<T, true /*moveConstruct*/,  true /*moveAssign*/, copyConstruct, copyAssign, valueConstruct, defaultConstruct, destructable>
{
	Navi(Navi&& other) = delete;

	// Inherit
	~Navi() = default;
	Navi() = default;
	using Navi<T, true /*moveConstruct*/, true /*moveAssign*/, copyConstruct, copyAssign, valueConstruct, defaultConstruct, destructable>::Navi;
	Navi& operator=(const Navi& other) = default;
	Navi(const Navi& other) = default;
	Navi& operator=(Navi&& other) = default;
};

// Non-move-constructable
template<typename T, bool moveAssign, bool copyConstruct, bool copyAssign, bool valueConstruct, bool defaultConstruct, bool destructable>
struct Navi<T, false /*moveConstruct*/, moveAssign, copyConstruct, copyAssign, valueConstruct, defaultConstruct, destructable>
     : Navi<T,  true /*moveConstruct*/, moveAssign, copyConstruct, copyAssign, valueConstruct, defaultConstruct, destructable>
{
	Navi& operator=(Navi&& other) = delete;

	// Inherit
	~Navi() = default;
	Navi() = default;
	using Navi<T, true /*moveConstruct*/, moveAssign, copyConstruct, copyAssign, valueConstruct, defaultConstruct, destructable>::Navi;
	Navi& operator=(const Navi& other) = default;
	Navi(const Navi& other) = default;
	Navi(Navi&& other) = default;
};

#pragma warning( pop ) 

namespace std
{
	template<typename T, bool... traits>
	struct hash<Navi<T, traits...>>
	{
		size_t operator()(const Navi<T, traits...>& navi) const noexcept
		{
			return hash<T>()(navi.value);
		}
	};
}
