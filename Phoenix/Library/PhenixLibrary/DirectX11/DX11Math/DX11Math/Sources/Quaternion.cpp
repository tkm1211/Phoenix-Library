#include "Quaternion.h"
#include "DX11Math.h"


namespace Phoenix
{
	namespace Lib
	{
#pragma region Quaternion
		/*Quaternion& Quaternion::operator += (const Quaternion& q)
		{
			x += q.x;
			y += q.y;
			z += q.z;
			w += q.w;

			return *this;
		}

		Quaternion& Quaternion::operator -= (const Quaternion& q)
		{
			x -= q.x;
			y -= q.y;
			z -= q.z;
			w -= q.w;

			return *this;
		}

		inline Quaternion& Quaternion::operator *= (const Quaternion& q)
		{
			Quaternion rq = QuaternionMultiply(*this, q);
			
			x = rq.x;
			y = rq.y;
			z = rq.z;
			w = rq.w;

			return *this;
		}

		Quaternion& Quaternion::operator *= (float f)
		{
			x *= f;
			y *= f;
			z *= f;
			w *= f;

			return *this;
		}

		Quaternion& Quaternion::operator /= (float f)
		{
			x /= f;
			y /= f;
			z /= f;
			w /= f;

			return *this;
		}

		inline Quaternion Quaternion::operator + () const
		{
			return *this;
		}

		inline Quaternion Quaternion::operator - () const
		{
			return Quaternion(-x, -y, -z, -w);
		}

		inline Quaternion Quaternion::operator + (const Quaternion& q) const
		{
			return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
		}

		inline Quaternion Quaternion::operator - (const Quaternion& q) const
		{
			return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
		}

		inline Quaternion Quaternion::operator * (const Quaternion& q) const
		{
			return QuaternionMultiply(*this, q);
		}

		inline Quaternion Quaternion::operator * (float f) const
		{
			return Quaternion(x * f, y * f, z * f, w * f);
		}

		inline Quaternion Quaternion::operator / (float f) const
		{
			return Quaternion(x * f, y * f, z * f, w * f);
		}

		inline Quaternion operator * (float f, const Quaternion& q)
		{
			return Quaternion(f * q.x, f * q.y, f * q.z, f * q.w);
		}

		bool Quaternion::operator == (const Quaternion& q) const
		{
			return x == q.x && y == q.y && z == q.z && w == q.w;
		}

		bool Quaternion::operator != (const Quaternion& q) const
		{
			return x != q.x || y != q.y || z != q.z || w != q.w;
		}*/

		inline Quaternion::Quaternion(const float* pf)
		{
			x = pf[0];
			y = pf[1];
			z = pf[2];
			w = pf[3];
		}

		inline Quaternion::Quaternion(float fx, float fy, float fz, float fw)
		{
			x = fx;
			y = fy;
			z = fz;
			w = fw;
		}

		inline Quaternion Quaternion::Mul(const Quaternion& q) const
		{
			return Quaternion
			{
				(w * q.x) + (x * q.w) + (y * q.z) - (z * q.y),
				(w * q.y) - (x * q.z) + (y * q.w) + (z * q.x),
				(w * q.z) + (x * q.y) - (y * q.x) + (z * q.w),
				(w * q.w) - (x * q.x) - (y * q.y) - (z * q.z)
			};
		}

		inline Vector3 Quaternion::RotateVector(const Vector3& target)
		{
			const Quaternion V = Quaternion(target.x, target.y, target.z, 1.0f);
			const Quaternion C = Quaternion(-x, -y, -z, w);

			Quaternion rotated;
			rotated = Mul(V);				// R = Q * V
			rotated = rotated.Mul(C);		// R = R * C

			return { rotated.x, rotated.y, rotated.z };
		}

		inline Vector3 Quaternion::LocalFront()
		{
			return RotateVector(Vector3(0.0f, 0.0f, 1.0f));
		}

		inline Vector3 Quaternion::LocalUp()
		{
			return RotateVector(Vector3(0.0f, 1.0f, 0.0f));
		}

		inline Vector3 Quaternion::LocalRight()
		{
			return RotateVector(Vector3(1.0f, 0.0f, 0.0f));
		}

		inline Vector4x4 Quaternion::RequireRotationMatrix()
		{
			Vector4x4 m;

			m._11 = 1.0f - (2.0f * y * y) - (2.0f * z * z);
			m._12 = (2.0f * x * y) + (2.0f * w * z);
			m._13 = (2.0f * x * z) - (2.0f * w * y);
			m._14 = 0.0f;

			m._21 = (2.0f * x * y) - (2.0f * w * z);
			m._22 = 1.0f - (2.0f * x * x) - (2.0f * z * z);
			m._23 = (2.0f * y * z) + (2.0f * w * x);
			m._24 = 0.0f;

			m._31 = (2.0f * x * z) + (2.0f * w * y);
			m._32 = (2.0f * y * z) - (2.0f * w * x);
			m._33 = 1.0f - (2.0f * x * x) - (2.0f * y * y);
			m._34 = 0.0f;

			m._41 = 0.0f;
			m._42 = 0.0f;
			m._43 = 0.0f;
			m._44 = 1.0f;

			return m;
		}

		inline Quaternion::operator float* ()
		{
			return (float*)&x;
		}

		inline Quaternion::operator const float* () const
		{
			return (const float*)&x;
		}

		inline Quaternion& Quaternion::operator += (const Quaternion& q)
		{
			x += q.x;
			y += q.y;
			z += q.z;
			w += q.w;

			return *this;
		}

		inline Quaternion& Quaternion::operator -= (const Quaternion& q)
		{
			x -= q.x;
			y -= q.y;
			z -= q.z;
			w -= q.w;

			return *this;
		}

		inline Quaternion& Quaternion::operator *= (const Quaternion& q)
		{
			Quaternion rq = QuaternionMultiply(*this, q);
			
			x = rq.x;
			y = rq.y;
			z = rq.z;
			w = rq.w;

			return *this;
		}

		inline Quaternion& Quaternion::operator *= (float f)
		{
			x *= f;
			y *= f;
			z *= f;
			w *= f;

			return *this;
		}

		inline Quaternion& Quaternion::operator /= (float f)
		{
			x /= f;
			y /= f;
			z /= f;
			w /= f;

			return *this;
		}

		inline Quaternion Quaternion::operator + () const
		{
			return *this;
		}

		inline Quaternion Quaternion::operator - () const
		{
			return Quaternion(-x, -y, -z, -w);
		}

		inline Quaternion Quaternion::operator + (const Quaternion& q) const
		{
			return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
		}

		inline Quaternion Quaternion::operator - (const Quaternion& q) const
		{
			return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
		}

		inline Quaternion Quaternion::operator * (const Quaternion& q) const
		{
			return QuaternionMultiply(*this, q);
		}

		inline Quaternion Quaternion::operator * (float f) const
		{
			return Quaternion(x * f, y * f, z * f, w * f);
		}

		inline Quaternion Quaternion::operator / (float f) const
		{
			return Quaternion(x * f, y * f, z * f, w * f);
		}

		inline Quaternion operator * (float f, const Quaternion& q)
		{
			return Quaternion(f * q.x, f * q.y, f * q.z, f * q.w);
		}

		inline bool Quaternion::operator == (const Quaternion& q) const
		{
			return x == q.x && y == q.y && z == q.z && w == q.w;
		}

		inline bool Quaternion::operator != (const Quaternion& q) const
		{
			return x != q.x || y != q.y || z != q.z || w != q.w;
		}

//#pragma region DirectX11
//		inline Quaternion QuaternionMultiply(const Quaternion& q1, const Quaternion& q2)
//		{
//			Quaternion rq;
//
//			DirectX::XMVECTOR vq1 = DirectX::XMVectorSet(q1.x, q1.y, q1.z, q1.w);
//			DirectX::XMVECTOR vq2 = DirectX::XMVectorSet(q2.x, q2.y, q2.z, q2.w);
//			DirectX::XMVECTOR rvq = DirectX::XMQuaternionMultiply(vq1, vq2);
//
//			DirectX::XMFLOAT4 fq;
//			DirectX::XMStoreFloat4(&fq, rvq);
//
//			rq.x = fq.x;
//			rq.y = fq.y;
//			rq.z = fq.z;
//			rq.w = fq.w;
//
//			return rq;
//		}
//
//		inline Quaternion QuaternionRotationAxis(Vector3 axis, float angle)
//		{
//			Quaternion rq;
//
//			DirectX::XMVECTOR vq = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(axis.x, axis.y, axis.z, 0.0f), angle);
//
//			DirectX::XMFLOAT4 fq;
//			DirectX::XMStoreFloat4(&fq, vq);
//
//			rq.x = fq.x;
//			rq.y = fq.y;
//			rq.z = fq.z;
//			rq.w = fq.w;
//
//			return rq;
//		}
//#pragma endregion
#pragma endregion
	}
}