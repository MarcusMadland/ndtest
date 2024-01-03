/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_FLOAT4X4_H_HEADER_GUARD
#define BASE_FLOAT4X4_H_HEADER_GUARD

#include "simd_t.h"

namespace base
{
	/// 4x4 matrix.
	BASE_ALIGN_DECL_16(struct) float4x4_t
	{
		simd128_t col[4];
	};

	/// Multiplies vector `_a` with matrix `_b` ignoring W component of vector `_a`.
	simd128_t simd_mul_xyz1(simd128_t _a, const float4x4_t* _b);

	/// Multiplies vector `_a` with matrix `_b`.
	simd128_t simd_mul(simd128_t _a, const float4x4_t* _b);

	/// Multiplies two matrices.
	void float4x4_mul(float4x4_t* _result, const float4x4_t* _a, const float4x4_t* _b);

	/// Multiplies two 3x4 affine matrices (i.e. "model" or "world" matrices).
	/// This function is a micro-optimized version of float4x4_mul() in the case
	/// when the last row of the both input matrices are (0, 0, 0, 1).
	void model4x4_mul(float4x4_t* _result, const float4x4_t* _a, const float4x4_t* _b);

	/// Multiplies a 3x4 affine matrix with a general 4x4 matrix.
	/// This function is a micro-optimized version of float4x4_mul() in the case
	/// when the last row of the _model input matrix is (0, 0, 0, 1).
	void model4x4_mul_viewproj4x4(float4x4_t* _result, const float4x4_t* _model, const float4x4_t* _viewProj);

	/// Transpose of matrix.
	void float4x4_transpose(float4x4_t* _result, const float4x4_t* _mtx);

	/// Inverse of matrix.
	void float4x4_inverse(float4x4_t* _result, const float4x4_t* _a);

} // namespace base

#include "inline/float4x4_t.inl"

#endif // BASE_FLOAT4X4_H_HEADER_GUARD
