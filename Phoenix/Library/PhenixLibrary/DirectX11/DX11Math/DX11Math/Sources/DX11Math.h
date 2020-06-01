#pragma once

// **************************
//	DirectX11
// **************************

#include <DirectXMath.h>

#include "DX11MathDefine.h"
#include "Vector.h"
#include "Quaternion.h"


namespace Phoenix
{
	namespace Lib
	{
#pragma region Functions for Vector2
		extern DirectX::XMFLOAT2 ConvertToFloat2FromVector2(const Vector2 v);
		extern DirectX::XMVECTOR ConvertToVectorFromVector2(const Vector2 v);
#pragma endregion

#pragma region Functions for Vector3
		extern DirectX::XMFLOAT3 ConvertToFloat3FromVector3(const Vector3 v);
		extern DirectX::XMVECTOR ConvertToVectorFromVector3(const Vector3 v);
#pragma endregion

#pragma region Functions for Vector4
		extern DirectX::XMFLOAT4 ConvertToFloat4FromVector4(const Vector4 v);
		extern DirectX::XMVECTOR ConvertToVectorFromVector4(const Vector4 v);
#pragma endregion

#pragma region Functions for Vector4x4
		extern DirectX::XMFLOAT4X4 ConvertToFloat4x4FromVector4x4(const Vector4x4 m);
		extern DirectX::XMMATRIX ConvertToMatrixFromVector4x4(const Vector4x4 m);

		extern Vector4x4 ConvertToVector4x4FromFloat4x4(const DirectX::XMFLOAT4X4& m);
		extern Vector4x4 ConvertToVector4x4FromMatrix(const DirectX::XMMATRIX& m);

		extern Vector4x4 MatrixMultiply(const Vector4x4 m1, const Vector4x4 m2);
#pragma endregion

#pragma region Functions for Quaternion
		extern DirectX::XMVECTOR ConvertToVectorFromQuaternion(const Quaternion q);
		extern Quaternion ConvertToQuaternionFromVector(const DirectX::XMVECTOR& v);

		extern Quaternion ConvertToQuaternionFromRotationMatrix(const Vector4x4 m);
		extern Vector4x4 ConvertToRotationMatrixFromQuaternion(const Quaternion q);

		//extern Quaternion QuaternionMultiply(const Quaternion q1, const Quaternion q2);
		//extern Quaternion QuaternionRotationAxis(Vector4 axis, float angle);

		/*extern Vector4 RotateVector(const Quaternion q, const Vector4& target);
		extern Vector4 RotateFrontVector(const Quaternion q);
		extern Vector4 RotateUpVector(const Quaternion q);
		extern Vector4 RotateRightVector(const Quaternion q);*/

		extern Quaternion QuaternionMultiply(const Quaternion& q1, const Quaternion& q2);
		extern Quaternion QuaternionRotationAxis(Vector3 axis, float angle);
#pragma endregion
	}
}