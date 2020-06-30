#pragma once

#include <DirectXMath.h>
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "AABB.h"
#include "Color.h"


namespace Phoenix
{
	namespace Math
	{
#pragma region Functions for Vector2
		extern DirectX::XMFLOAT2 ConvertToFloat2FromVector2(const Vector2 v);
		extern DirectX::XMVECTOR ConvertToVectorFromVector2(const Vector2 v);
#pragma endregion

#pragma region Functions for Vector3
		extern DirectX::XMFLOAT3 ConvertToFloat3FromVector3(const Vector3 v);
		extern DirectX::XMVECTOR ConvertToVectorFromVector3(const Vector3 v);

		extern Vector3 ConvertToVector3FromFloat3(const DirectX::XMFLOAT3& f);
		extern Vector3 ConvertToVector3FromVector(const DirectX::XMVECTOR& v);

		extern Vector3 Vector3Normalize(const Vector3 v);

		// 2��3D�x�N�g�������Z����B
		extern Vector3 Vector3Subtract(const Vector3 v1, const Vector3 v2);

		// 2��3D�x�N�g���̓��ς��v�Z����
		extern f32 Vector3Dot(const Vector3 v1, const Vector3 v2);

		// 2��3D�x�N�g���̊O�ς��v�Z����B
		extern Vector3 Vector3Cross(const Vector3 v1, const Vector3 v2);

		extern Vector3 Vector3Lerp(const Vector3 v1, const Vector3 v2, f32 s);

		// �w�肳�ꂽ�s��ɂ�� 3D �x�N�g�����g�����X�t�H�[�����A���̌��ʂ� w = 1 �Ɏˉe����B
		extern Vector3 Vector3TransformCoord(const Vector3 v, const Matrix m);

		// 3D�x�N�g���̒�����Ԃ��B
		extern f32 Vector3Length(const Vector3 v);
#pragma endregion

#pragma region Functions for Vector4
		extern DirectX::XMFLOAT4 ConvertToFloat4FromVector4(const Vector4 v);
		extern DirectX::XMVECTOR ConvertToVectorFromVector4(const Vector4 v);
#pragma endregion

#pragma region Functions for Matrix
		extern DirectX::XMFLOAT4X4 ConvertToFloat4x4FromVector4x4(const Matrix m);
		extern DirectX::XMMATRIX ConvertToMatrixFromVector4x4(const Matrix m);

		extern Matrix ConvertToVector4x4FromFloat4x4(const DirectX::XMFLOAT4X4& m);
		extern Matrix ConvertToVector4x4FromMatrix(const DirectX::XMMATRIX& m);

		extern Matrix MatrixIdentity();
		extern Matrix MatrixInverse(const Matrix m);
		extern Matrix MatrixMultiply(const Matrix m1, const Matrix m2);
		extern Matrix MatrixTranspose(const Matrix m);
		extern Matrix MatrixMultiplyTranspose(const Matrix m1, const Matrix m2);

		// ���s�ړ��I�t�Z�b�g���w�肵�čs����쐬����B
		Matrix MatrixTranslation(f32 x, f32 y, f32 z);

		// x ���Ay ���Az ���ɉ����ăX�P�[�����O����s����쐬����B
		Matrix MatrixScaling(f32 sx, f32 sy, f32 sz);

		// x ������]���ɂ��ĉ�]����s����쐬����B
		Matrix MatrixRotationX(f32 angle);

		// y ������]���ɂ��ĉ�]����s����쐬����B
		Matrix MatrixRotationY(f32 angle);

		// z ������]���ɂ��ĉ�]����s����쐬����B
		Matrix MatrixRotationZ(f32 angle);

		// ���[�A�s�b�`�A����у��[�����w�肵�čs����쐬����B
		Matrix MatrixRotationYawPitchRoll(f32 yaw, f32 pitch, f32 roll);

		// �N�H�[�^�j�I�������]�s����쐬����B
		Matrix MatrixRotationQuaternion(const Quaternion* q);

		// ����Ɋ�Â��āA�E����W�n�p�[�X�y�N�e�B�u�ˉe�s����쐬����B
		extern Matrix MatrixPerspectiveFov(f32 fovY, f32 aspect, f32 zn, f32 zf);

		// �J�X�^�}�C�Y�����E����W�n���ˉe�s����쐬����B
		extern Matrix MatrixOrthoOffCenter(f32 l, f32 r, f32 b, f32 t, f32 zn, f32 zf);

		// �E����W�n�r���[�s����쐬����B
		extern Matrix MatrixLookAt(const Vector3 eye, const Vector3 at, const Vector3 up);

		// �E����W�n���ˉe�s����쐬����B
		extern Matrix MatrixOrtho(f32 w, f32 h, f32 zn, f32 zf);
#pragma endregion

#pragma region Functions for Quaternion
		extern DirectX::XMVECTOR ConvertToVectorFromQuaternion(const Quaternion q);
		extern Quaternion ConvertToQuaternionFromVector(const DirectX::XMVECTOR& v);

		extern Quaternion ConvertToQuaternionFromRotationMatrix(const Matrix m);
		extern Matrix ConvertToRotationMatrixFromQuaternion(const Quaternion q);

		//extern Quaternion QuaternionMultiply(const Quaternion q1, const Quaternion q2);
		//extern Quaternion QuaternionRotationAxis(Vector4 axis, float angle);

		/*extern Vector4 RotateVector(const Quaternion q, const Vector4& target);
		extern Vector4 RotateFrontVector(const Quaternion q);
		extern Vector4 RotateUpVector(const Quaternion q);
		extern Vector4 RotateRightVector(const Quaternion q);*/

		extern f32 QuaternionDot(const Quaternion q1, const Quaternion q2);
		extern Quaternion QuaternionMultiply(const Quaternion& q1, const Quaternion& q2);
		extern Quaternion QuaternionRotationAxis(Vector3 axis, float angle);
		extern Quaternion QuaternionSlerp(const Quaternion q1, const Quaternion q2, f32 t);
#pragma endregion
	} // namespace Math
} // namespace Phoenix