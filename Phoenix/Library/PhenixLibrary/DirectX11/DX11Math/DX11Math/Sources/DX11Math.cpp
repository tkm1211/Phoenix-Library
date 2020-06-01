#include "DX11Math.h"


namespace Phoenix
{
	namespace Lib
	{
#pragma region Functions for Vector2
		inline DirectX::XMFLOAT2 ConvertToFloat2FromVector2(const Vector2 v)
		{
			DirectX::XMFLOAT2 f2;

			f2.x = v.x;
			f2.y = v.y;

			return f2;
		}

		inline DirectX::XMVECTOR ConvertToVectorFromVector2(const Vector2 v)
		{
			DirectX::XMVECTOR v2;
			DirectX::XMFLOAT2 f2;

			f2.x = v.x;
			f2.y = v.y;

			v2 = DirectX::XMLoadFloat2(&f2);

			return v2;
		}
#pragma endregion

#pragma region Functions for Vector3
		inline DirectX::XMFLOAT3 ConvertToFloat3FromVector3(const Vector3 v)
		{
			DirectX::XMFLOAT3 f3;

			f3.x = v.x;
			f3.y = v.y;
			f3.z = v.z;

			return f3;
		}

		inline DirectX::XMVECTOR ConvertToVectorFromVector3(const Vector3 v)
		{
			DirectX::XMVECTOR v3;
			DirectX::XMFLOAT3 f3;

			f3.x = v.x;
			f3.y = v.y;
			f3.z = v.z;

			v3 = DirectX::XMLoadFloat3(&f3);

			return v3;
		}
#pragma endregion

#pragma region Functions for Vector4
		inline DirectX::XMFLOAT4 ConvertToFloat4FromVector4(const Vector4 v)
		{
			DirectX::XMFLOAT4 f4;

			f4.x = v.x;
			f4.y = v.y;
			f4.z = v.z;
			f4.w = v.w;

			return f4;
		}

		inline DirectX::XMVECTOR ConvertToVectorFromVector4(const Vector4 v)
		{
			DirectX::XMVECTOR v4;
			DirectX::XMFLOAT4 f4;

			f4.x = v.x;
			f4.y = v.y;
			f4.z = v.z;
			f4.w = v.w;

			v4 = DirectX::XMLoadFloat4(&f4);

			return v4;
		}
#pragma endregion

#pragma region Functions for Vector4x4
		inline DirectX::XMFLOAT4X4 ConvertToFloat4x4FromVector4x4(const Vector4x4 m)
		{
			DirectX::XMFLOAT4X4 mT;

			mT._11 = m._11; mT._12 = m._12; mT._13 = m._13; mT._14 = m._14;
			mT._21 = m._21; mT._22 = m._22; mT._23 = m._23; mT._24 = m._24;
			mT._31 = m._31; mT._32 = m._32; mT._33 = m._33; mT._34 = m._34;
			mT._41 = m._41; mT._42 = m._42; mT._43 = m._43; mT._44 = m._44;

			return mT;
		}

		inline DirectX::XMMATRIX ConvertToMatrixFromVector4x4(const Vector4x4 m)
		{
			DirectX::XMFLOAT4X4 mT;
			DirectX::XMMATRIX rM;

			mT._11 = m._11; mT._12 = m._12; mT._13 = m._13; mT._14 = m._14;
			mT._21 = m._21; mT._22 = m._22; mT._23 = m._23; mT._24 = m._24;
			mT._31 = m._31; mT._32 = m._32; mT._33 = m._33; mT._34 = m._34;
			mT._41 = m._41; mT._42 = m._42; mT._43 = m._43; mT._44 = m._44;

			rM = DirectX::XMLoadFloat4x4(&mT);

			return rM;
		}

		inline Vector4x4 ConvertToVector4x4FromFloat4x4(const DirectX::XMFLOAT4X4& m)
		{
			Vector4x4 mT;

			mT._11 = m._11; mT._12 = m._12; mT._13 = m._13; mT._14 = m._14;
			mT._21 = m._21; mT._22 = m._22; mT._23 = m._23; mT._24 = m._24;
			mT._31 = m._31; mT._32 = m._32; mT._33 = m._33; mT._34 = m._34;
			mT._41 = m._41; mT._42 = m._42; mT._43 = m._43; mT._44 = m._44;

			return mT;
		}

		inline Vector4x4 ConvertToVector4x4FromMatrix(const DirectX::XMMATRIX& m)
		{
			Vector4x4 rM;
			DirectX::XMFLOAT4X4 mT;
			
			DirectX::XMStoreFloat4x4(&mT, m);

			rM._11 = mT._11; rM._12 = mT._12; rM._13 = mT._13; rM._14 = mT._14;
			rM._21 = mT._21; rM._22 = mT._22; rM._23 = mT._23; rM._24 = mT._24;
			rM._31 = mT._31; rM._32 = mT._32; rM._33 = mT._33; rM._34 = mT._34;
			rM._41 = mT._41; rM._42 = mT._42; rM._43 = mT._43; rM._44 = mT._44;

			return rM;
		}

		inline Vector4x4 MatrixMultiply(const Vector4x4 m1, const Vector4x4 m2)
		{
			DirectX::XMMATRIX m1T;
			DirectX::XMMATRIX m2T;
			DirectX::XMMATRIX rM;

			m1T = ConvertToMatrixFromVector4x4(m1);
			m2T = ConvertToMatrixFromVector4x4(m2);

			rM = DirectX::XMMatrixMultiply(m1T, m2T);

			return ConvertToVector4x4FromMatrix(rM);
		}
#pragma endregion

#pragma region Functions for Quaternion
		inline DirectX::XMVECTOR ConvertToVectorFromQuaternion(const Quaternion q)
		{
			DirectX::XMVECTOR v = DirectX::XMVectorSet(q.x, q.y, q.z, q.w);
	
			return v;
		}
	
		inline Quaternion ConvertToQuaternionFromVector(const  DirectX::XMVECTOR& v)
		{
			Quaternion q;
	
			DirectX::XMFLOAT4 f;
			DirectX::XMStoreFloat4(&f, v);
	
			q.x = f.x;
			q.y = f.y;
			q.z = f.z;
			q.w = f.w;
	
			return q;
		}
	
		inline Quaternion ConvertToQuaternionFromRotationMatrix(const Vector4x4 m)
		{
			DirectX::XMVECTOR v;
			DirectX::XMMATRIX mT = ConvertToMatrixFromVector4x4(m);
	
			v = DirectX::XMQuaternionRotationMatrix(mT);
	
			return ConvertToQuaternionFromVector(v);
		}
	
		inline Vector4x4 ConvertToRotationMatrixFromQuaternion(const Quaternion q)
		{
			DirectX::XMVECTOR v = ConvertToVectorFromQuaternion(q);
			DirectX::XMMATRIX r = DirectX::XMMatrixRotationQuaternion(v);
	
			return ConvertToVector4x4FromMatrix(r);
		}
//
//		inline Quaternion QuaternionMultiply(const Quaternion q1, const Quaternion q2)
//		{
//#if 0
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
//#else
//			return Quaternion
//			{
//				(q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y),
//				(q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x),
//				(q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w),
//				(q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z)
//			};
//#endif
//		}
//
//		inline Quaternion QuaternionRotationAxis(Vector4 axis, float angle)
//		{
//			Quaternion rq;
//
//			Vector4 axisT = axis;
//			axisT.w = 0.0f;
//
//			DirectX::XMVECTOR vT = ConvertToVectorFromVector4(axisT);
//			DirectX::XMVECTOR vq = DirectX::XMQuaternionRotationAxis(vT, angle);
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
//
//		inline Vector4 RotateVector(const Quaternion q, const Vector4& target)
//		{
//			const Quaternion v = Quaternion(target.x, target.y, target.z, target.w);
//			const Quaternion c = Quaternion(-q.x, -q.y, -q.z, q.w);
//
//			Quaternion rotated;
//			rotated = QuaternionMultiply(q, v);			// R = Q * V
//			rotated = QuaternionMultiply(rotated, c);	// R = R * C
//
//			return Vector4(rotated.x, rotated.y, rotated.z, rotated.w);
//		}
//
//		inline Vector4 RotateFrontVector(const Quaternion q)
//		{
//#if 0
//			return RotateVector(q, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
//#else
//			Vector4x4 rM = ConvertToRotationMatrixFromQuaternion(q);
//
//			return Vector4(rM._31, rM._32, rM._33, 1.0f);
//#endif
//		}
//
//		inline Vector4 RotateUpVector(const Quaternion q)
//		{
//#if 0
//			return RotateVector(q, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
//#else
//			Vector4x4 rM = ConvertToRotationMatrixFromQuaternion(q);
//
//			return Vector4(rM._21, rM._22, rM._23, 1.0f);
//#endif
//		}
//
//		inline Vector4 RotateRightVector(const Quaternion q)
//		{
//#if 0
//			return RotateVector(q, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
//#else
//			Vector4x4 rM = ConvertToRotationMatrixFromQuaternion(q);
//
//			return Vector4(rM._11, rM._12, rM._13, 1.0f);
//#endif
//		}

		inline Quaternion QuaternionMultiply(const Quaternion& q1, const Quaternion& q2)
		{
			Quaternion rq;

			DirectX::XMVECTOR vq1 = DirectX::XMVectorSet(q1.x, q1.y, q1.z, q1.w);
			DirectX::XMVECTOR vq2 = DirectX::XMVectorSet(q2.x, q2.y, q2.z, q2.w);
			DirectX::XMVECTOR rvq = DirectX::XMQuaternionMultiply(vq1, vq2);

			DirectX::XMFLOAT4 fq;
			DirectX::XMStoreFloat4(&fq, rvq);

			rq.x = fq.x;
			rq.y = fq.y;
			rq.z = fq.z;
			rq.w = fq.w;

			return rq;
		}

		inline Quaternion QuaternionRotationAxis(Vector3 axis, float angle)
		{
			Quaternion rq;

			DirectX::XMVECTOR vq = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(axis.x, axis.y, axis.z, 0.0f), angle);

			DirectX::XMFLOAT4 fq;
			DirectX::XMStoreFloat4(&fq, vq);

			rq.x = fq.x;
			rq.y = fq.y;
			rq.z = fq.z;
			rq.w = fq.w;

			return rq;
		}
#pragma endregion
	}
}