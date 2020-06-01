#include "Vector.h"
#include "DX11Math.h"


namespace Phoenix
{
	namespace Lib
	{
#pragma region Vector2
		inline Vector2 Vector2::operator- () const
		{
			Vector2 v;

			v.x = -x;
			v.y = -y;

			return v;
		}

		inline Vector2& Vector2::operator += (const Vector2 v)
		{
			x = x + v.x;
			y = y + v.y;

			return *this;
		}

		inline Vector2& Vector2::operator -= (const Vector2 v)
		{
			x = x - v.x;
			y = y - v.y;

			return *this;
		}

		inline Vector2& Vector2::operator *= (const Vector2 v)
		{
			x = x * v.x;
			y = y * v.y;

			return *this;
		}

		inline Vector2& Vector2::operator /= (const Vector2 v)
		{
			x = x / v.x;
			y = y / v.y;

			return *this;
		}

		inline Vector2& Vector2::operator *= (float f)
		{
			x = x * f;
			y = y * f;

			return *this;
		}

		inline Vector2& Vector2::operator /= (float f)
		{
			x = x / f;
			y = y / f;

			return *this;
		}

		inline Vector2 Vector2::operator + (const Vector2 v) const
		{
			Vector2 vT;

			vT.x = x + v.x;
			vT.y = y + v.y;

			return vT;
		}

		inline Vector2 Vector2::operator - (const Vector2 v) const
		{
			Vector2 vT;

			vT.x = x - v.x;
			vT.y = y - v.y;

			return vT;
		}

		inline Vector2 Vector2::operator * (const Vector2 v) const
		{
			Vector2 vT;

			vT.x = x * v.x;
			vT.y = y * v.y;

			return vT;
		}

		inline Vector2 Vector2::operator / (const Vector2 v) const
		{
			Vector2 vT;

			vT.x = x / v.x;
			vT.y = y / v.y;

			return vT;
		}

		inline Vector2 Vector2::operator * (float f) const
		{
			Vector2 vT;

			vT.x = x * f;
			vT.y = y * f;

			return vT;
		}

		inline Vector2 Vector2::operator / (float f) const
		{
			Vector2 vT;

			vT.x = x / f;
			vT.y = y / f;

			return vT;
		}

		inline Vector2 operator * (float f, const Vector2 v)
		{
			Vector2 vT;

			vT.x = f * v.x;
			vT.y = f * v.y;

			return vT;
		}
#pragma endregion

#pragma region Vector3
		inline Vector3 Vector3::operator- () const
		{
			Vector3 v;

			v.x = -x;
			v.y = -y;
			v.z = -z;

			return v;
		}

		inline Vector3& Vector3::operator += (const Vector3 v)
		{
			x = x + v.x;
			y = y + v.y;
			z = z + v.z;

			return *this;
		}

		inline Vector3& Vector3::operator -= (const Vector3 v)
		{
			x = x - v.x;
			y = y - v.y;
			z = z - v.z;

			return *this;
		}

		inline Vector3& Vector3::operator *= (const Vector3 v)
		{
			x = x * v.x;
			y = y * v.y;
			z = z * v.z;

			return *this;
		}

		inline Vector3& Vector3::operator /= (const Vector3 v)
		{
			x = x / v.x;
			y = y / v.y;
			z = z / v.z;

			return *this;
		}

		inline Vector3& Vector3::operator *= (float f)
		{
			x = x * f;
			y = y * f;
			z = z * f;

			return *this;
		}

		inline Vector3& Vector3::operator /= (float f)
		{
			x = x / f;
			y = y / f;
			z = z / f;

			return *this;
		}

		inline Vector3 Vector3::operator + (const Vector3 v) const
		{
			Vector3 vT;

			vT.x = x + v.x;
			vT.y = y + v.y;
			vT.z = z + v.z;

			return vT;
		}

		inline Vector3 Vector3::operator - (const Vector3 v) const
		{
			Vector3 vT;

			vT.x = x - v.x;
			vT.y = y - v.y;
			vT.z = z - v.z;

			return vT;
		}

		inline Vector3 Vector3::operator * (const Vector3 v) const
		{
			Vector3 vT;

			vT.x = x * v.x;
			vT.y = y * v.y;
			vT.z = z * v.z;

			return vT;
		}

		inline Vector3 Vector3::operator / (const Vector3 v) const
		{
			Vector3 vT;

			vT.x = x / v.x;
			vT.y = y / v.y;
			vT.z = z / v.z;

			return vT;
		}

		inline Vector3 Vector3::operator * (float f) const
		{
			Vector3 vT;

			vT.x = x * f;
			vT.y = y * f;
			vT.z = z * f;

			return vT;
		}

		inline Vector3 Vector3::operator / (float f) const
		{
			Vector3 vT;

			vT.x = x / f;
			vT.y = y / f;
			vT.z = z / f;

			return vT;
		}

		inline Vector3 operator * (float f, const Vector3 v)
		{
			Vector3 vT;

			vT.x = f * v.x;
			vT.y = f * v.y;
			vT.z = f * v.z;

			return vT;
		}
#pragma endregion

#pragma region Vector4
		inline Vector4 Vector4::operator- () const
		{
			Vector4 v;

			v.x = -x;
			v.y = -y;
			v.z = -z;
			v.w = -w;

			return v;
		}

		inline Vector4& Vector4::operator += (const Vector4 v)
		{
			x = x + v.x;
			y = y + v.y;
			z = z + v.z;
			w = w + v.w;

			return *this;
		}

		inline Vector4& Vector4::operator -= (const Vector4 v)
		{
			x = x - v.x;
			y = y - v.y;
			z = z - v.z;
			w = w - v.w;

			return *this;
		}

		inline Vector4& Vector4::operator *= (const Vector4 v)
		{
			x = x * v.x;
			y = y * v.y;
			z = z * v.z;
			w = w * v.w;

			return *this;
		}

		inline Vector4& Vector4::operator /= (const Vector4 v)
		{
			x = x / v.x;
			y = y / v.y;
			z = z / v.z;
			w = w / v.w;

			return *this;
		}

		inline Vector4& Vector4::operator *= (float f)
		{
			x = x * f;
			y = y * f;
			z = z * f;
			w = w * f;

			return *this;
		}

		inline Vector4& Vector4::operator /= (float f)
		{
			x = x / f;
			y = y / f;
			z = z / f;
			w = w / f;

			return *this;
		}

		inline Vector4 Vector4::operator + (const Vector4 v) const
		{
			Vector4 vT;

			vT.x = x + v.x;
			vT.y = y + v.y;
			vT.z = z + v.z;
			vT.w = w + v.w;

			return vT;
		}

		inline Vector4 Vector4::operator - (const Vector4 v) const
		{
			Vector4 vT;

			vT.x = x - v.x;
			vT.y = y - v.y;
			vT.z = z - v.z;
			vT.w = w - v.w;

			return vT;
		}

		inline Vector4 Vector4::operator * (const Vector4 v) const
		{
			Vector4 vT;

			vT.x = x * v.x;
			vT.y = y * v.y;
			vT.z = z * v.z;
			vT.w = w * v.w;

			return vT;
		}

		inline Vector4 Vector4::operator / (const Vector4 v) const
		{
			Vector4 vT;

			vT.x = x / v.x;
			vT.y = y / v.y;
			vT.z = z / v.z;
			vT.w = w / v.w;

			return vT;
		}

		inline Vector4 Vector4::operator * (float f) const
		{
			Vector4 vT;

			vT.x = x * f;
			vT.y = y * f;
			vT.z = z * f;
			vT.w = w * f;

			return vT;
		}

		inline Vector4 Vector4::operator / (float f) const
		{
			Vector4 vT;

			vT.x = x / f;
			vT.y = y / f;
			vT.z = z / f;
			vT.w = w / f;

			return vT;
		}

		inline Vector4 operator * (float f, const Vector4 v)
		{
			Vector4 vT;

			vT.x = f * v.x;
			vT.y = f * v.y;
			vT.z = f * v.z;
			vT.w = f * v.w;

			return vT;
		}
#pragma endregion

#pragma region Vector4x4
		inline float& Vector4x4::operator () (unsigned int row, unsigned int col)
		{
			return m[row][col];
		}

		inline float  Vector4x4::operator () (unsigned int row, unsigned int col) const
		{
			return m[row][col];
		}

		inline Vector4x4& Vector4x4::operator += (const Vector4x4& mat)
		{
			_11 += mat._11; _12 += mat._12; _13 += mat._13; _14 += mat._14;
			_21 += mat._21; _22 += mat._22; _23 += mat._23; _24 += mat._24;
			_31 += mat._31; _32 += mat._32; _33 += mat._33; _34 += mat._34;
			_41 += mat._41; _42 += mat._42; _43 += mat._43; _44 += mat._44;

			return *this;
		}

		inline Vector4x4& Vector4x4::operator -= (const Vector4x4& mat)
		{
			_11 -= mat._11; _12 -= mat._12; _13 -= mat._13; _14 -= mat._14;
			_21 -= mat._21; _22 -= mat._22; _23 -= mat._23; _24 -= mat._24;
			_31 -= mat._31; _32 -= mat._32; _33 -= mat._33; _34 -= mat._34;
			_41 -= mat._41; _42 -= mat._42; _43 -= mat._43; _44 -= mat._44;

			return *this;
		}

		inline Vector4x4& Vector4x4::operator *= (float f)
		{
			_11 *= f; _12 *= f; _13 *= f; _14 *= f;
			_21 *= f; _22 *= f; _23 *= f; _24 *= f;
			_31 *= f; _32 *= f; _33 *= f; _34 *= f;
			_41 *= f; _42 *= f; _43 *= f; _44 *= f;

			return *this;
		}

		inline Vector4x4& Vector4x4::operator *= (const Vector4x4& mat)
		{
			*this = MatrixMultiply(*this, mat);

			return *this;
		}

		Vector4x4& Vector4x4::operator /= (float f)
		{
			_11 /= f; _12 /= f; _13 /= f; _14 /= f;
			_21 /= f; _22 /= f; _23 /= f; _24 /= f;
			_31 /= f; _32 /= f; _33 /= f; _34 /= f;
			_41 /= f; _42 /= f; _43 /= f; _44 /= f;

			return *this;
		}

		Vector4x4& Vector4x4::operator = (const Vector4x4& mat)
		{
			_11 = mat._11; _12 = mat._12; _13 = mat._13; _14 = mat._14;
			_21 = mat._21; _22 = mat._22; _23 = mat._23; _24 = mat._24;
			_31 = mat._31; _32 = mat._32; _33 = mat._33; _34 = mat._34;
			_41 = mat._41; _42 = mat._42; _43 = mat._43; _44 = mat._44;

			return *this;
		}

		Vector4x4 Vector4x4::operator + () const
		{
			return *this;
		}

		inline Vector4x4 Vector4x4::operator - () const
		{
			return Vector4x4
			(
				-_11, -_12, -_13, -_14,
				-_21, -_22, -_23, -_24,
				-_31, -_32, -_33, -_34,
				-_41, -_42, -_43, -_44
			);
		}

		inline Vector4x4 Vector4x4::operator + (const Vector4x4& mat) const
		{
			return Vector4x4
			(
				_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
				_21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
				_31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
				_41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44
			);
		}

		inline  Vector4x4 Vector4x4::operator - (const Vector4x4& mat) const
		{
			return Vector4x4
			(
				_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
				_21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
				_31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
				_41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44
			);
		}

		inline Vector4x4 Vector4x4::operator * (float f) const
		{
			return Vector4x4
			(
				_11 * f, _12 * f, _13 * f, _14 * f,
				_21 * f, _22 * f, _23 * f, _24 * f,
				_31 * f, _32 * f, _33 * f, _34 * f,
				_41 * f, _42 * f, _43 * f, _44 * f
			);
		}

		inline Vector4x4 Vector4x4::operator * (const Vector4x4& mat) const
		{
			Vector4x4 matT;

			matT = MatrixMultiply(*this, mat);

			return matT;
		}

		inline Vector4x4 Vector4x4::operator / (float f) const
		{
			return Vector4x4
			(
				_11 / f, _12 / f, _13 / f, _14 / f,
				_21 / f, _22 / f, _23 / f, _24 / f,
				_31 / f, _32 / f, _33 / f, _34 / f,
				_41 / f, _42 / f, _43 / f, _44 / f
			);
		}

		inline Vector4x4 operator * (float f, const Vector4x4& mat)
		{
			return Vector4x4
			(
				f * mat._11, f * mat._12, f * mat._13, f * mat._14,
				f * mat._21, f * mat._22, f * mat._23, f * mat._24,
				f * mat._31, f * mat._32, f * mat._33, f * mat._34,
				f * mat._41, f * mat._42, f * mat._43, f * mat._44
			);
		}

		bool Vector4x4::operator == (const Vector4x4& mat) const
		{
			return 0 == memcmp(this, &mat, sizeof(Vector4x4));
		}

		bool Vector4x4::operator != (const Vector4x4& mat) const
		{
			return 0 != memcmp(this, &mat, sizeof(Vector4x4));
		}
#pragma endregion
	}
}