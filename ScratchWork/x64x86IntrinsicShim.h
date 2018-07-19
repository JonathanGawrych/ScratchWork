#pragma once

#ifdef _WIN64
#include <intrin.h>
#pragma intrinsic(_umul128)
#pragma intrinsic(_addcarry_u64)

#else

#include <cstdint>

// MSVC doesn't optimize 32x32 => 64b multiplication without its intrinsic
// But good compilers can just use this to get a single mul instruction
static inline uint64_t __emulu(uint32_t x, uint32_t y)
{
	return x * (uint64_t) y;
}

// https://stackoverflow.com/a/46924301/1248889
// This is still pretty ugly with MSVC, branching on the carry
//  and using XMM store / integer reload to zero a register!
// But at least it inlines 4 mul instructions
//  instead of calling a generic 64x64 => 64b multiply helper function
#pragma warning(push)
#pragma warning(disable: 4244) // conversion from 'uint64_t' to 'uint32_t', possible loss of data, intentional
uint64_t _umul128(uint64_t multiplier, uint64_t multiplicand, uint64_t *product_hi)
{
	// multiplier   = ab = a * 2^32 + b
	// multiplicand = cd = c * 2^32 + d
	// ab * cd = a * c * 2^64 + (a * d + b * c) * 2^32 + b * d
	uint64_t a = multiplier >> 32;
	uint64_t b = (uint32_t) multiplier; // & 0xFFFFFFFF;
	uint64_t c = multiplicand >> 32;
	uint64_t d = (uint32_t) multiplicand; // & 0xFFFFFFFF;

	//uint64_t ac = __emulu(a, c);
	uint64_t ad = __emulu(a, d);
	//uint64_t bc = __emulu(b, c);
	uint64_t bd = __emulu(b, d);

	uint64_t adbc = ad + __emulu(b, c);
	uint64_t adbc_carry = (adbc < ad); // ? 1 : 0;
	// MSVC gets confused by the ternary and makes worse code than using a boolean in an integer context for 1 : 0

	// multiplier * multiplicand = product_hi * 2^64 + product_lo
	uint64_t product_lo = bd + (adbc << 32);
	uint64_t product_lo_carry = (product_lo < bd); // ? 1 : 0;
	*product_hi = __emulu(a, c) + (adbc >> 32) + (adbc_carry << 32) + product_lo_carry;

	return product_lo;
}
#pragma warning(pop)

unsigned char _addcarry_u64(unsigned char c_in, unsigned __int64 a, unsigned __int64 b, unsigned __int64 * out)
{
	*out = a + b;
	return c_in || *out < a;
}

#endif

