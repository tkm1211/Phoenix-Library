#pragma once

#include <memory>

#include "DX11MathDefine.h"


namespace Phoenix
{
	namespace Lib
	{
#pragma region Vector2
		struct DX11MATH_API Vector2
		{
			float x, y;

			Vector2() = default;
			Vector2(Vector2&&) = default;
			Vector2(const Vector2&) = default;
			Vector2(const float* pf) : x(pf[0]), y(pf[1]) {}
			Vector2(float fx, float fy) : x(fx), y(fy) {}

			Vector2& operator = (const Vector2&) = default;
			Vector2& operator = (Vector2&&) = default;

			Vector2 operator+ () const { return *this; }
			Vector2 operator- () const;

			Vector2& operator += (const Vector2 v);
			Vector2& operator -= (const Vector2 v);
			Vector2& operator *= (const Vector2 v);
			Vector2& operator /= (const Vector2 v);

			Vector2& operator *= (float f);
			Vector2& operator /= (float f);

			Vector2 operator + (const Vector2 v) const;
			Vector2 operator - (const Vector2 v) const;
			Vector2 operator * (const Vector2 v) const;
			Vector2 operator / (const Vector2 v) const;

			Vector2 operator * (float f) const;
			Vector2 operator / (float f) const;

			friend Vector2 operator * (float f, const Vector2 v);
		};
#pragma endregion

#pragma region Vector3
		struct DX11MATH_API Vector3
		{
			float x, y, z;

			Vector3() = default;
			Vector3(Vector3&&) = default;
			Vector3(const Vector3&) = default;
			Vector3(const float* pf) : x(pf[0]), y(pf[1]), z(pf[2]) {}
			Vector3(float fx, float fy, float fz) : x(fx), y(fy), z(fz) {}

			Vector3& operator = (const Vector3&) = default;
			Vector3& operator = (Vector3&&) = default;

			Vector3 operator+ () const { return *this; }
			Vector3 operator- () const;

			Vector3& operator += (const Vector3 v);
			Vector3& operator -= (const Vector3 v);
			Vector3& operator *= (const Vector3 v);
			Vector3& operator /= (const Vector3 v);

			Vector3& operator *= (float f);
			Vector3& operator /= (float f);

			Vector3 operator + (const Vector3 v) const;
			Vector3 operator - (const Vector3 v) const;
			Vector3 operator * (const Vector3 v) const;
			Vector3 operator / (const Vector3 v) const;

			Vector3 operator * (float f) const;
			Vector3 operator / (float f) const;

			friend Vector3 operator * (float f, const Vector3 v);
		};
#pragma endregion

#pragma region Vector4
		struct DX11MATH_API Vector4
		{
			float x, y, z, w;

			Vector4() = default;
			Vector4(Vector4&&) = default;
			Vector4(const Vector4&) = default;
			Vector4(const float* pf) : x(pf[0]), y(pf[1]), z(pf[2]), w(pf[3]) {}
			Vector4(float fx, float fy, float fz, float fw) : x(fx), y(fy), z(fz), w(fw) {}

			Vector4& operator = (const Vector4&) = default;
			Vector4& operator = (Vector4&&) = default;

			Vector4 operator+ () const { return *this; }
			Vector4 operator- () const;

			Vector4& operator += (const Vector4 v);
			Vector4& operator -= (const Vector4 v);
			Vector4& operator *= (const Vector4 v);
			Vector4& operator /= (const Vector4 v);

			Vector4& operator *= (float f);
			Vector4& operator /= (float f);

			Vector4 operator + (const Vector4 v) const;
			Vector4 operator - (const Vector4 v) const;
			Vector4 operator * (const Vector4 v) const;
			Vector4 operator / (const Vector4 v) const;

			Vector4 operator * (float f) const;
			Vector4 operator / (float f) const;

			friend Vector4 operator * (float f, const Vector4 v);
		};
#pragma endregion

#pragma region Vector4x4
		struct DX11MATH_API Vector4x4
		{
			union
			{
				struct
				{
					float _11, _12, _13, _14;
					float _21, _22, _23, _24;
					float _31, _32, _33, _34;
					float _41, _42, _43, _44;
				};
				float m[4][4];
			};

			Vector4x4() = default;
			Vector4x4(const float* pf) :
				_11(pf[0]),  _12(pf[1]),  _13(pf[2]),  _14(pf[3]),
				_21(pf[4]),  _22(pf[5]),  _23(pf[6]),  _24(pf[7]),
				_31(pf[8]),  _32(pf[9]),  _33(pf[10]), _34(pf[11]),
				_41(pf[12]), _42(pf[13]), _43(pf[14]), _44(pf[15]) {}
			Vector4x4
			(
				float f11, float f12, float f13, float f14,
				float f21, float f22, float f23, float f24,
				float f31, float f32, float f33, float f34,
				float f41, float f42, float f43, float f44
			) :
				_11(f11), _12(f12), _13(f13), _14(f14),
				_21(f21), _22(f22), _23(f23), _24(f24),
				_31(f31), _32(f32), _33(f33), _34(f34),
				_41(f41), _42(f42), _43(f43), _44(f44) {}
			~Vector4x4() {}

		public:
			float& operator () (unsigned int row, unsigned int col);
			float  operator () (unsigned int row, unsigned int col) const;

		public:
			Vector4x4& operator += (const Vector4x4& mat);
			Vector4x4& operator -= (const Vector4x4& mat);
			Vector4x4& operator *= (float f);
			Vector4x4& operator *= (const Vector4x4& mat);
			Vector4x4& operator /= (float f);

			Vector4x4& operator = (const Vector4x4& mat);

			Vector4x4 operator + () const;
			Vector4x4 operator - () const;

			Vector4x4 operator + (const Vector4x4& mat) const;
			Vector4x4 operator - (const Vector4x4& mat) const;
			Vector4x4 operator * (float f) const;
			Vector4x4 operator * (const Vector4x4& mat) const;
			Vector4x4 operator / (float f) const;

			friend Vector4x4 operator * (float f, const Vector4x4& mat);

			bool operator == (const Vector4x4& mat) const;
			bool operator != (const Vector4x4& mat) const;
		};
#pragma endregion
	}
}
