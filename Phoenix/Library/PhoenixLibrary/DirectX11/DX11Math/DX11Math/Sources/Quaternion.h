#pragma once

#include "DX11MathDefine.h"
#include "Vector.h"

//ConvertToVectorFromQuaternion
//ConvertToQuaternionFromVector
//ConvertToQuaternionFromRotationMatrix
//ConvertToRotationMatrixFromQuaternion
//QuaternionMultiply
//QuaternionRotationAxis
//RotateVector
//RotateFrontVector
//RotateUpVector
//RotateRightVector


namespace Phoenix
{
	namespace Lib
	{
#pragma region Quaternion
		struct DX11MATH_API Quaternion
		{
		public:
			float x, y, z, w;

		public:
			Quaternion() {}
			Quaternion(const float* pf);
			Quaternion(float fx, float fy, float fz, float fw);
			~Quaternion() {}

		public:
			Quaternion Mul(const Quaternion& q) const;

			Vector3 RotateVector(const Vector3& target);
			Vector3 LocalFront();
			Vector3 LocalUp();
			Vector3 LocalRight();

			Vector4x4 RequireRotationMatrix();

		public:
			operator float* ();
			operator const float* () const;

		public:
			Quaternion& operator += (const Quaternion& q);
			Quaternion& operator -= (const Quaternion& q);
			Quaternion& operator *= (const Quaternion& q);
			Quaternion& operator *= (float f);
			Quaternion& operator /= (float f);

			Quaternion  operator + () const;
			Quaternion  operator - () const;

			Quaternion operator + (const Quaternion& q) const;
			Quaternion operator - (const Quaternion& q) const;
			Quaternion operator * (const Quaternion& q) const;
			Quaternion operator * (float f) const;
			Quaternion operator / (float f) const;

			friend Quaternion operator * (float f, const Quaternion& q);

			bool operator == (const Quaternion& q) const;
			bool operator != (const Quaternion& q) const;
		};
#pragma endregion

	}
}