/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_SIMD_T_H_HEADER_GUARD
#define BASE_SIMD_T_H_HEADER_GUARD

#include "base.h"

#define BASE_SIMD_FORCE_INLINE BASE_FORCE_INLINE
#define BASE_SIMD_INLINE inline

#define BASE_SIMD_AVX     0
#define BASE_SIMD_LANGEXT 0
#define BASE_SIMD_NEON    0
#define BASE_SIMD_SSE     0

#define BASE_CONFIG_SUPPORTS_SIMD 0

#if defined(__AVX__) || defined(__AVX2__)
#	include <immintrin.h>
#	undef  BASE_SIMD_AVX
#	define BASE_SIMD_AVX 1
#endif //

#if defined(__SSE2__) || (BASE_COMPILER_MSVC && (BASE_ARCH_64BIT || _M_IX86_FP >= 2) )
#	include <emmintrin.h> // __m128i
#	if defined(__SSE4_1__)
#		include <smmintrin.h>
#	endif // defined(__SSE4_1__)
#	include <xmmintrin.h> // __m128
#	undef  BASE_SIMD_SSE
#	define BASE_SIMD_SSE 1
#elif defined(__ARM_NEON__) && (!BASE_COMPILER_CLANG || BASE_CLANG_HAS_EXTENSION(attribute_ext_vector_type) )
#	include <arm_neon.h>
#	undef  BASE_SIMD_NEON
#	define BASE_SIMD_NEON 1
#elif   BASE_COMPILER_CLANG \
	&& !BASE_PLATFORM_EMSCRIPTEN \
	&& !BASE_PLATFORM_IOS \
	&&  BASE_CLANG_HAS_EXTENSION(attribute_ext_vector_type)
#	undef  BASE_SIMD_LANGEXT
#	define BASE_SIMD_LANGEXT 1
#endif //

namespace base
{
#define ELEMx 0
#define ELEMy 1
#define ELEMz 2
#define ELEMw 3
#define BASE_SIMD128_IMPLEMENT_SWIZZLE(_x, _y, _z, _w) \
			template<typename Ty> \
			Ty simd_swiz_##_x##_y##_z##_w(Ty _a);
#include "inline/simd128_swizzle.inl"

#undef BASE_SIMD128_IMPLEMENT_SWIZZLE
#undef ELEMw
#undef ELEMz
#undef ELEMy
#undef ELEMx

#define BASE_SIMD128_IMPLEMENT_TEST(_xyzw) \
			template<typename Ty> \
			BASE_SIMD_FORCE_INLINE bool simd_test_any_##_xyzw(Ty _test); \
			\
			template<typename Ty> \
			BASE_SIMD_FORCE_INLINE bool simd_test_all_##_xyzw(Ty _test)

BASE_SIMD128_IMPLEMENT_TEST(x   );
BASE_SIMD128_IMPLEMENT_TEST(y   );
BASE_SIMD128_IMPLEMENT_TEST(xy  );
BASE_SIMD128_IMPLEMENT_TEST(z   );
BASE_SIMD128_IMPLEMENT_TEST(xz  );
BASE_SIMD128_IMPLEMENT_TEST(yz  );
BASE_SIMD128_IMPLEMENT_TEST(xyz );
BASE_SIMD128_IMPLEMENT_TEST(w   );
BASE_SIMD128_IMPLEMENT_TEST(xw  );
BASE_SIMD128_IMPLEMENT_TEST(yw  );
BASE_SIMD128_IMPLEMENT_TEST(xyw );
BASE_SIMD128_IMPLEMENT_TEST(zw  );
BASE_SIMD128_IMPLEMENT_TEST(xzw );
BASE_SIMD128_IMPLEMENT_TEST(yzw );
BASE_SIMD128_IMPLEMENT_TEST(xyzw);
#undef BASE_SIMD128_IMPLEMENT_TEST

	template<typename Ty>
	Ty simd_shuf_xyAB(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_shuf_ABxy(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_shuf_CDzw(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_shuf_zwCD(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_shuf_xAyB(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_shuf_AxBy(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_shuf_zCwD(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_shuf_CzDw(Ty _a, Ty _b);

	template<typename Ty>
	float simd_x(Ty _a);

	template<typename Ty>
	float simd_y(Ty _a);

	template<typename Ty>
	float simd_z(Ty _a);

	template<typename Ty>
	float simd_w(Ty _a);

	template<typename Ty>
	Ty simd_ld(const void* _ptr);

	template<typename Ty>
	void simd_st(void* _ptr, Ty _a);

	template<typename Ty>
	void simd_stx(void* _ptr, Ty _a);

	template<typename Ty>
	void simd_stream(void* _ptr, Ty _a);

	template<typename Ty>
	Ty simd_ld(float _x, float _y, float _z, float _w);

	template<typename Ty>
	Ty simd_ld(float _x, float _y, float _z, float _w, float _a, float _b, float _c, float _d);

	template<typename Ty>
	Ty simd_ild(uint32_t _x, uint32_t _y, uint32_t _z, uint32_t _w);

	template<typename Ty>
	Ty simd_ild(uint32_t _x, uint32_t _y, uint32_t _z, uint32_t _w, uint32_t _a, uint32_t _b, uint32_t _c, uint32_t _d);

	template<typename Ty>
	Ty simd_splat(const void* _ptr);

	template<typename Ty>
	Ty simd_splat(float _a);

	template<typename Ty>
	Ty simd_isplat(uint32_t _a);

	template<typename Ty>
	Ty simd_zero();

	template<typename Ty>
	Ty simd_itof(Ty _a);

	template<typename Ty>
	Ty simd_ftoi(Ty _a);

	template<typename Ty>
	Ty simd_round(Ty _a);

	template<typename Ty>
	Ty simd_add(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_sub(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_mul(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_div(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_rcp_est(Ty _a);

	template<typename Ty>
	Ty simd_sqrt(Ty _a);

	template<typename Ty>
	Ty simd_rsqrt_est(Ty _a);

	template<typename Ty>
	Ty simd_dot3(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_dot(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_cmpeq(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_cmpneq(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_cmplt(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_cmple(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_cmpgt(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_cmpge(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_min(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_max(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_and(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_andc(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_or(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_xor(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_sll(Ty _a, int _count);

	template<typename Ty>
	Ty simd_srl(Ty _a, int _count);

	template<typename Ty>
	Ty simd_sra(Ty _a, int _count);

	template<typename Ty>
	Ty simd_icmpeq(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_icmplt(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_icmpgt(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_imin(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_imax(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_iadd(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_isub(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_shuf_xAzC(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_shuf_yBwD(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_rcp(Ty _a);

	template<typename Ty>
	Ty simd_orx(Ty _a);

	template<typename Ty>
	Ty simd_orc(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_neg(Ty _a);

	template<typename Ty>
	Ty simd_madd(Ty _a, Ty _b, Ty _c);

	template<typename Ty>
	Ty simd_nmsub(Ty _a, Ty _b, Ty _c);

	template<typename Ty>
	Ty simd_div_nr(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_selb(Ty _mask, Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_sels(Ty _test, Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_not(Ty _a);

	template<typename Ty>
	Ty simd_abs(Ty _a);

	template<typename Ty>
	Ty simd_clamp(Ty _a, Ty _min, Ty _max);

	template<typename Ty>
	Ty simd_lerp(Ty _a, Ty _b, Ty _s);

	template<typename Ty>
	Ty simd_rsqrt(Ty _a);

	template<typename Ty>
	Ty simd_rsqrt_nr(Ty _a);

	template<typename Ty>
	Ty simd_rsqrt_carmack(Ty _a);

	template<typename Ty>
	Ty simd_sqrt_nr(Ty _a);

	template<typename Ty>
	Ty simd_log2(Ty _a);

	template<typename Ty>
	Ty simd_exp2(Ty _a);

	template<typename Ty>
	Ty simd_pow(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_cross3(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_normalize3(Ty _a);

	template<typename Ty>
	Ty simd_ceil(Ty _a);

	template<typename Ty>
	Ty simd_floor(Ty _a);

	template<typename Ty>
	Ty simd_shuf_xAzC_ni(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_shuf_yBwD_ni(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_madd_ni(Ty _a, Ty _b, Ty _c);

	template<typename Ty>
	Ty simd_nmsub_ni(Ty _a, Ty _b, Ty _c);

	template<typename Ty>
	Ty simd_div_nr_ni(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_rcp_ni(Ty _a);

	template<typename Ty>
	Ty simd_orx_ni(Ty _a);

	template<typename Ty>
	Ty simd_orc_ni(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_neg_ni(Ty _a);

	template<typename Ty>
	Ty simd_selb_ni(Ty _mask, Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_sels_ni(Ty _test, Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_not_ni(Ty _a);

	template<typename Ty>
	Ty simd_cmpneq_ni(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_min_ni(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_max_ni(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_abs_ni(Ty _a);

	template<typename Ty>
	Ty simd_imin_ni(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_imax_ni(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_clamp_ni(Ty _a, Ty _min, Ty _max);

	template<typename Ty>
	Ty simd_lerp_ni(Ty _a, Ty _b, Ty _s);

	template<typename Ty>
	Ty simd_sqrt_nr_ni(Ty _a);

	template<typename Ty>
	Ty simd_sqrt_nr1_ni(Ty _a);

	template<typename Ty>
	Ty simd_rsqrt_ni(Ty _a);

	template<typename Ty>
	Ty simd_rsqrt_nr_ni(Ty _a);

	template<typename Ty>
	Ty simd_rsqrt_carmack_ni(Ty _a);

	template<typename Ty>
	Ty simd_log2_ni(Ty _a);

	template<typename Ty>
	Ty simd_exp2_ni(Ty _a);

	template<typename Ty>
	Ty simd_pow_ni(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_dot3_ni(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_cross3_ni(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_normalize3_ni(Ty _a);

	template<typename Ty>
	Ty simd_dot_ni(Ty _a, Ty _b);

	template<typename Ty>
	Ty simd_ceil_ni(Ty _a);

	template<typename Ty>
	Ty simd_floor_ni(Ty _a);

	template<typename Ty>
	Ty simd_round_ni(Ty _a);

	template<typename Ty>
	bool simd_test_any_ni(Ty _a);

	template<typename Ty>
	bool simd_test_all_ni(Ty _a);

#if BASE_SIMD_AVX
	typedef __m256 simd256_avx_t;
#endif // BASE_SIMD_SSE

#if BASE_SIMD_LANGEXT
	union simd128_langext_t
	{
		float    __attribute__((vector_size(16))) vf;
		int32_t  __attribute__((vector_size(16))) vi;
		uint32_t __attribute__((vector_size(16))) vu;
		float    fxyzw[4];
		int32_t  ixyzw[4];
		uint32_t uxyzw[4];

	};
#endif // BASE_SIMD_LANGEXT

#if BASE_SIMD_NEON
	typedef float32x4_t simd128_neon_t;
#endif // BASE_SIMD_NEON

#if BASE_SIMD_SSE
	typedef __m128 simd128_sse_t;
#endif // BASE_SIMD_SSE

} // namespace base

#if BASE_SIMD_AVX
#	include "inline/simd256_avx.inl"
#endif // BASE_SIMD_AVX

#if BASE_SIMD_LANGEXT
#	include "inline/simd128_langext.inl"
#endif // BASE_SIMD_LANGEXT

#if BASE_SIMD_NEON
#	include "inline/simd128_neon.inl"
#endif // BASE_SIMD_NEON

#if BASE_SIMD_SSE
#	include "inline/simd128_sse.inl"
#endif // BASE_SIMD_SSE

#if (  BASE_SIMD_LANGEXT \
	|| BASE_SIMD_NEON    \
	|| BASE_SIMD_SSE     \
	|| BASE_SIMD_AVX     \
	)
#	undef  BASE_CONFIG_SUPPORTS_SIMD
#	define BASE_CONFIG_SUPPORTS_SIMD 1
#endif // BASE_SIMD_*

namespace base
{
	union simd128_ref_t
	{
		float    fxyzw[4];
		int32_t  ixyzw[4];
		uint32_t uxyzw[4];
	};

#ifndef BASE_SIMD_WARN_REFERENCE_IMPL
#	define BASE_SIMD_WARN_REFERENCE_IMPL 0
#endif // BASE_SIMD_WARN_REFERENCE_IMPL

#if !BASE_CONFIG_SUPPORTS_SIMD
#	if BASE_SIMD_WARN_REFERENCE_IMPL
#		pragma message("*** Using SIMD128 reference implementation! ***")
#	endif // BASE_SIMD_WARN_REFERENCE_IMPL

	typedef simd128_ref_t simd128_t;
#endif // BASE_SIMD_REFERENCE

	struct simd256_ref_t
	{
#if BASE_COMPILER_MSVC
		typedef simd128_ref_t type;
#else
		typedef simd128_t type;
#endif // BASE_COMPILER_MSVC

		type simd128_0;
		type simd128_1;
	};

#if !BASE_SIMD_AVX
#	if BASE_SIMD_WARN_REFERENCE_IMPL
#		pragma message("*** Using SIMD256 reference implementation! ***")
#	endif // BASE_SIMD_WARN_REFERENCE_IMPL

	typedef simd256_ref_t simd256_t;
#endif // !BASE_SIMD_AVX

	simd128_t simd_zero();

	simd128_t simd_ld(const void* _ptr);

	simd128_t simd_ld(float _x, float _y, float _z, float _w);

	simd128_t simd_ild(uint32_t _x, uint32_t _y, uint32_t _z, uint32_t _w);

	simd128_t simd_splat(const void* _ptr);

	simd128_t simd_splat(float _a);

	simd128_t simd_isplat(uint32_t _a);

} // namespace base

#include "inline/simd128_ref.inl"
#include "inline/simd256_ref.inl"

#include "inline/simd_ni.inl"

#endif // BASE_SIMD_T_H_HEADER_GUARD
