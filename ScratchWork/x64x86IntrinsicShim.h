#pragma once

#include <intrin.h>

// 64-bit MSVC || Clang
#if defined(_M_X64) || defined(__x86_64__)
#pragma intrinsic(_umul128)

#ifdef __clang__
#pragma intrinsic(__builtin_ia32_addcarry_u64)
#define _addcarry_u64 __builtin_ia32_addcarry_u64
#else
#pragma intrinsic(_addcarry_u64)
#endif

#else

#include <cstdint> // std::uint64_t, std::uint32_t

// https://stackoverflow.com/a/46924301/1248889
// This is still pretty ugly with MSVC, branching on the carry
//  and using XMM store / integer reload to zero a register!
// But at least it inlines 4 mul instructions
//  instead of calling a generic 64x64 => 64b multiply helper function
uint64_t _umul128(uint64_t multiplier, uint64_t multiplicand, uint64_t *product_hi)
{
	// multiplier   = ab = a * 2^32 + b
	// multiplicand = cd = c * 2^32 + d
	// ab * cd = a * c * 2^64 + (a * d + b * c) * 2^32 + b * d
	uint64_t a = multiplier >> 32;
	uint64_t b = static_cast<uint32_t>(multiplier); // & 0xFFFFFFFF;
	uint64_t c = multiplicand >> 32;
	uint64_t d = static_cast<uint32_t>(multiplicand); // & 0xFFFFFFFF;

	//uint64_t ac = __emulu(a, c);
	uint64_t ad = __emulu(static_cast<uint32_t>(a), static_cast<uint32_t>(d));
	//uint64_t bc = __emulu(b, c);
	uint64_t bd = __emulu(static_cast<uint32_t>(b), static_cast<uint32_t>(d));

	uint64_t adbc = ad + __emulu(static_cast<uint32_t>(b), static_cast<uint32_t>(c));
	uint64_t adbc_carry = (adbc < ad); // ? 1 : 0;
	// MSVC gets confused by the ternary and makes worse code than using a boolean in an integer context for 1 : 0

	// multiplier * multiplicand = product_hi * 2^64 + product_lo
	uint64_t product_lo = bd + (adbc << 32);
	uint64_t product_lo_carry = (product_lo < bd); // ? 1 : 0;
	*product_hi = __emulu(static_cast<uint32_t>(a), static_cast<uint32_t>(c)) + (adbc >> 32) + (adbc_carry << 32) + product_lo_carry;

	return product_lo;
}

unsigned char _addcarry_u64(unsigned char c_in, uint64_t a, uint64_t b, uint64_t * out)
{
	*out = a + b;
	return c_in || *out < a;
}

#endif

