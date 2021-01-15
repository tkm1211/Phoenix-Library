#include "pch.h"
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/Math/Arithmetic.h"


namespace Phoenix
{
	namespace Math
	{
#pragma region Functions for f32
		f32 f32Lerp(const f32 f1, const f32 f2, f32 s)
		{
			f32 f;
			f = (1.0f - s) * (f1)+s * (f2);

			return f;
		}
#pragma endregion

#pragma region Functions for Vector2
		DirectX::XMFLOAT2 ConvertToFloat2FromVector2(const Vector2 v)
		{
			DirectX::XMFLOAT2 f2;

			f2.x = v.x;
			f2.y = v.y;

			return f2;
		}

		DirectX::XMVECTOR ConvertToVectorFromVector2(const Vector2 v)
		{
			DirectX::XMVECTOR v2;
			DirectX::XMFLOAT2 f2;

			f2.x = v.x;
			f2.y = v.y;

			v2 = DirectX::XMLoadFloat2(&f2);

			return v2;
		}

		Vector2 ConvertToVector2FromVector(const DirectX::XMVECTOR& v)
		{
			Vector2 v2;

			DirectX::XMFLOAT2 f2;
			DirectX::XMStoreFloat2(&f2, v);

			v2.x = f2.x;
			v2.y = f2.y;

			return v2;
		}

		f32 Vector2Length(const Vector2 v)
		{
			return SqrtF32((v.x) * (v.x) + (v.y) * (v.y));
		}

		Vector2 Vector2Normalize(const Vector2 v)
		{
			DirectX::XMVECTOR vT = DirectX::XMVector2Normalize(ConvertToVectorFromVector2(v));
			return ConvertToVector2FromVector(vT);
		}

		Vector2 Vector2Lerp(const Vector2 v1, const Vector2 v2, f32 s)
		{
			Vector2 vT;

			vT.x = (1.0f - s) * (v1.x) + s * (v2.x);
			vT.y = (1.0f - s) * (v1.y) + s * (v2.y);

			return vT;
		}
#pragma endregion

#pragma region Functions for Vector3
		DirectX::XMFLOAT3 ConvertToFloat3FromVector3(const Vector3 v)
		{
			DirectX::XMFLOAT3 f3;

			f3.x = v.x;
			f3.y = v.y;
			f3.z = v.z;

			return f3;
		}

		DirectX::XMVECTOR ConvertToVectorFromVector3(const Vector3 v)
		{
			DirectX::XMVECTOR v3;
			DirectX::XMFLOAT3 f3;

			f3.x = v.x;
			f3.y = v.y;
			f3.z = v.z;

			v3 = DirectX::XMLoadFloat3(&f3);

			return v3;
		}

		Vector3 ConvertToVector3FromFloat3(const DirectX::XMFLOAT3& f)
		{
			Vector3 v3;

			v3.x = f.x;
			v3.y = f.y;
			v3.z = f.z;

			return v3;
		}

		Vector3 ConvertToVector3FromVector(const DirectX::XMVECTOR& v)
		{
			Vector3 v3;

			DirectX::XMFLOAT3 f3;
			DirectX::XMStoreFloat3(&f3, v);

			v3.x = f3.x;
			v3.y = f3.y;
			v3.z = f3.z;

			return v3;
		}

		extern Vector3 Vector3Normalize(const Vector3 v)
		{
			DirectX::XMVECTOR vT = DirectX::XMVector3Normalize(ConvertToVectorFromVector3(v));
			return ConvertToVector3FromVector(vT);
		}

		// 2��3D�x�N�g�������Z����B
		extern Vector3 Vector3Subtract(const Vector3 v1, const Vector3 v2)
		{
			Vector3 v;

			v.x = v1.x - v2.x;
			v.y = v1.y - v2.y;
			v.z = v1.z - v2.z;

			return v;
		}

		// 2��3D�x�N�g���̓��ς��v�Z����
		extern f32 Vector3Dot(const Vector3 v1, const Vector3 v2)
		{
			return (v1.x) * (v2.x) + (v1.y) * (v2.y) + (v1.z) * (v2.z);
		}

		// 2��3D�x�N�g���̊O�ς��v�Z����B
		extern Vector3 Vector3Cross(const Vector3 v1, const Vector3 v2)
		{
			Vector3 v;

			f32 x = (v1.y) * (v2.z) - (v1.z) * (v2.y);
			f32 y = (v1.z) * (v2.x) - (v1.x) * (v2.z);
			f32 z = (v1.x) * (v2.y) - (v1.y) * (v2.x);

			v.x = x;
			v.y = y;
			v.z = z;

			return v;
		}

		extern Vector3 Vector3Lerp(const Vector3 v1, const Vector3 v2, f32 s)
		{
			Vector3 vT;

			vT.x = (1.0f - s) * (v1.x) + s * (v2.x);
			vT.y = (1.0f - s) * (v1.y) + s * (v2.y);
			vT.z = (1.0f - s) * (v1.z) + s * (v2.z);

			return vT;
		}

		extern Vector3 Vector3SphereLinear(const Vector3 v1, const Vector3 v2, f32 s)
		{
			Vector3 start, end;
			start = Vector3Normalize(v1);
			end = Vector3Normalize(v2);

			f32 angle = acosf(Vector3Dot(start, end));

			f32 sinThita = sinf(angle);

			f32 startPoint = sinf(angle * (1.0f - s));
			f32 endPoint = sinf(angle * s);

			Vector3 outV = (startPoint * start + endPoint * end) / sinThita;

			return Vector3Normalize(outV);
		}

		extern Vector3 Vector3TransformCoord(const Vector3 v, const Matrix m)
		{
			Vector3 vT;

			f32 t = m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3];

			if (NotZero(t))
			{
				t = 1.0f / t;
				f32 x = (m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0]) * t;
				f32 y = (m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1]) * t;
				f32 z = (m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2]) * t;

				vT.x = x;
				vT.y = y;
				vT.z = z;
			}
			else
			{
				vT.x = 0.0f;
				vT.y = 0.0f;
				vT.z = 0.0f;
			}
			return vT;
		}

		// 3D�x�N�g���̒�����Ԃ��B
		extern f32 Vector3Length(const Vector3 v)
		{
			return SqrtF32((v.x) * (v.x) + (v.y) * (v.y) + (v.z) * (v.z));
		}


#pragma endregion

#pragma region Functions for Vector4
		DirectX::XMFLOAT4 ConvertToFloat4FromVector4(const Vector4 v)
		{
			DirectX::XMFLOAT4 f4;

			f4.x = v.x;
			f4.y = v.y;
			f4.z = v.z;
			f4.w = v.w;

			return f4;
		}

		DirectX::XMVECTOR ConvertToVectorFromVector4(const Vector4 v)
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

		Vector4 ConvertToVector4FromVector(const DirectX::XMVECTOR& v)
		{
			Vector4 v4;

			DirectX::XMFLOAT4 f4;
			DirectX::XMStoreFloat4(&f4, v);

			v4.x = f4.x;
			v4.y = f4.y;
			v4.z = f4.z;
			v4.w = f4.w;

			return v4;
		}

		// 4D�x�N�g���̒�����Ԃ�
		f32 Vector4Length(const Vector4 v)
		{
			return SqrtF32((v.x) * (v.x) + (v.y) * (v.y) + (v.z) * (v.z) + (v.w) * (v.w));
		}

		// 4D�x�N�g���̐��K�������x�N�g����Ԃ�
		Vector4 Vector4Normalize(const Vector4 v)
		{
			Vector4 vT;

			f32 norm = Vector4Length(v);
			if (!norm)
			{
				vT.x = 0.0f;
				vT.y = 0.0f;
				vT.z = 0.0f;
				vT.w = 0.0f;
			}
			else
			{
				norm = 1.0f / norm;
				vT.x = v.x * norm;
				vT.y = v.y * norm;
				vT.z = v.z * norm;
				vT.w = v.w * norm;
			}
			return vT;
		}

		// 2��4D�x�N�g���̓��ς��v�Z����
		f32 Vector4Dot(const Vector4 v1, const Vector4 v2)
		{
			return (v1.x) * (v2.x) + (v1.y) * (v2.y) + (v1.z) * (v2.z) + (v1.w) * (v2.w);
		}

		Vector4 Vector4Rotate(const Vector4 v, const Quaternion q)
		{
			DirectX::XMVECTOR r = DirectX::XMVector3Rotate(ConvertToVectorFromVector4(v), ConvertToVectorFromQuaternion(q));
			return ConvertToVector4FromVector(r);
		}
#pragma endregion

#pragma region Functions for Vector4x4
		DirectX::XMFLOAT4X4 ConvertToFloat4x4FromVector4x4(const Matrix m)
		{
			DirectX::XMFLOAT4X4 mT;

			mT._11 = m._11; mT._12 = m._12; mT._13 = m._13; mT._14 = m._14;
			mT._21 = m._21; mT._22 = m._22; mT._23 = m._23; mT._24 = m._24;
			mT._31 = m._31; mT._32 = m._32; mT._33 = m._33; mT._34 = m._34;
			mT._41 = m._41; mT._42 = m._42; mT._43 = m._43; mT._44 = m._44;

			return mT;
		}

		DirectX::XMMATRIX ConvertToMatrixFromVector4x4(const Matrix m)
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

		Matrix ConvertToVector4x4FromFloat4x4(const DirectX::XMFLOAT4X4& m)
		{
			Matrix mT;

			mT._11 = m._11; mT._12 = m._12; mT._13 = m._13; mT._14 = m._14;
			mT._21 = m._21; mT._22 = m._22; mT._23 = m._23; mT._24 = m._24;
			mT._31 = m._31; mT._32 = m._32; mT._33 = m._33; mT._34 = m._34;
			mT._41 = m._41; mT._42 = m._42; mT._43 = m._43; mT._44 = m._44;

			return mT;
		}

		Matrix ConvertToVector4x4FromMatrix(const DirectX::XMMATRIX& m)
		{
			Matrix rM;
			DirectX::XMFLOAT4X4 mT;

			DirectX::XMStoreFloat4x4(&mT, m);

			rM._11 = mT._11; rM._12 = mT._12; rM._13 = mT._13; rM._14 = mT._14;
			rM._21 = mT._21; rM._22 = mT._22; rM._23 = mT._23; rM._24 = mT._24;
			rM._31 = mT._31; rM._32 = mT._32; rM._33 = mT._33; rM._34 = mT._34;
			rM._41 = mT._41; rM._42 = mT._42; rM._43 = mT._43; rM._44 = mT._44;

			return rM;
		}

		Matrix MatrixIdentity()
		{
			Matrix rM;

			rM._11 = 1.0f;
			rM._12 = 0.0f;
			rM._13 = 0.0f;
			rM._14 = 0.0f;

			rM._21 = 0.0f;
			rM._22 = 1.0f;
			rM._23 = 0.0f;
			rM._24 = 0.0f;

			rM._31 = 0.0f;
			rM._32 = 0.0f;
			rM._33 = 1.0f;
			rM._34 = 0.0f;

			rM._41 = 0.0f;
			rM._42 = 0.0f;
			rM._43 = 0.0f;
			rM._44= 1.0f;

			return rM;
		}

		Matrix MatrixInverse(const Matrix m)
		{
			DirectX::XMMATRIX mT;
			DirectX::XMMATRIX rM;

			mT = ConvertToMatrixFromVector4x4(m);
			rM = DirectX::XMMatrixInverse(nullptr, mT);

			return ConvertToVector4x4FromMatrix(rM);
		}

		Matrix MatrixMultiply(const Matrix m1, const Matrix m2)
		{
			DirectX::XMMATRIX m1T;
			DirectX::XMMATRIX m2T;
			DirectX::XMMATRIX rM;

			m1T = ConvertToMatrixFromVector4x4(m1);
			m2T = ConvertToMatrixFromVector4x4(m2);

			rM = DirectX::XMMatrixMultiply(m1T, m2T);

			return ConvertToVector4x4FromMatrix(rM);
		}

		Matrix MatrixTranspose(const Matrix m)
		{
			DirectX::XMMATRIX mT;
			DirectX::XMMATRIX rM;

			mT = ConvertToMatrixFromVector4x4(m);
			rM = DirectX::XMMatrixTranspose(mT);

			return ConvertToVector4x4FromMatrix(rM);
		}

		Matrix MatrixMultiplyTranspose(const Matrix m1, const Matrix m2)
		{
			return MatrixTranspose(MatrixMultiply(m1, m2));
		}

		// ���s�ړ��I�t�Z�b�g���w�肵�čs����쐬����B
		Matrix MatrixTranslation(f32 x, f32 y, f32 z)
		{
			Matrix rM;

			rM.m[0][0] = 1.0f;
			rM.m[0][1] = 0.0f;
			rM.m[0][2] = 0.0f;
			rM.m[0][3] = 0.0f;

			rM.m[1][0] = 0.0f;
			rM.m[1][1] = 1.0f;
			rM.m[1][2] = 0.0f;
			rM.m[1][3] = 0.0f;

			rM.m[2][0] = 0.0f;
			rM.m[2][1] = 0.0f;
			rM.m[2][2] = 1.0f;
			rM.m[2][3] = 0.0f;

			rM.m[3][0] = x;
			rM.m[3][1] = y;
			rM.m[3][2] = z;
			rM.m[3][3] = 1.0f;

			return rM;
		}

		// x ���Ay ���Az ���ɉ����ăX�P�[�����O����s����쐬����B
		Matrix MatrixScaling(f32 sx, f32 sy, f32 sz)
		{
			Matrix rM;
			rM = MatrixIdentity();

			rM.m[0][0] = sx;
			rM.m[1][1] = sy;
			rM.m[2][2] = sz;

			return rM;
		}

		// x ������]���ɂ��ĉ�]����s����쐬����
		Matrix MatrixRotationX(f32 angle)
		{
			Matrix rM;
			rM = MatrixIdentity();

			f32 c = CosF32(angle);
			f32 s = SinF32(angle);

			rM.m[1][1] = c;
			rM.m[2][2] = c;
			rM.m[1][2] = s;
			rM.m[2][1] = -s;

			return rM;
		}

		// y ������]���ɂ��ĉ�]����s����쐬����
		Matrix MatrixRotationY(f32 angle)
		{
			Matrix rM;
			rM = MatrixIdentity();

			f32 c = CosF32(angle);
			f32 s = SinF32(angle);

			rM.m[0][0] = c;
			rM.m[2][2] = c;
			rM.m[0][2] = -s;
			rM.m[2][0] = s;

			return rM;
		}

		// z ������]���ɂ��ĉ�]����s����쐬����
		Matrix MatrixRotationZ(f32 angle)
		{
			Matrix rM;
			rM = MatrixIdentity();

			f32 c = CosF32(angle);
			f32 s = SinF32(angle);

			rM.m[0][0] = c;
			rM.m[1][1] = c;
			rM.m[0][1] = s;
			rM.m[1][0] = -s;

			return rM;
		}

		// ���[���A�s�b�`�A����у��[���w�肵�čs����쐬����B
		Matrix MatrixRotationRollPitchYaw(f32 roll, f32 pitch, f32 yaw)
		{
			Matrix rM;
			Matrix m, out1, out2, out3;

			out3 = MatrixIdentity();
			m = MatrixRotationZ(yaw);
			out2 = MatrixMultiply(out3, m);
			m = MatrixRotationX(roll);
			out1 = MatrixMultiply(out2, m);
			m = MatrixRotationY(pitch);
			rM = MatrixMultiply(out1, m);

			return rM;
		}

		// ���[�A�s�b�`�A����у��[�����w�肵�čs����쐬����
		Matrix MatrixRotationYawPitchRoll(f32 yaw, f32 pitch, f32 roll)
		{
			Matrix rM;
			Matrix m, out1, out2, out3;

			out3 = MatrixIdentity();
			m = MatrixRotationZ(roll);
			out2 = MatrixMultiply(out3, m);
			m = MatrixRotationX(pitch);
			out1 = MatrixMultiply(out2, m);
			m = MatrixRotationY(yaw);
			rM = MatrixMultiply(out1, m);

			return rM;
		}

		// �N�H�[�^�j�I�������]�s����쐬����B
		Matrix MatrixRotationQuaternion(const Quaternion* q)
		{
			Matrix rM;
			rM = MatrixIdentity();

			/*rM.m[0][0] = 1.0f - 2.0f * (q->y * q->y + q->z * q->z);
			  rM.m[0][1] = 2.0f * (q->x * q->y + q->z * q->w);
			  rM.m[0][2] = 2.0f * (q->x * q->z - q->y * q->w);
			  rM.m[1][0] = 2.0f * (q->x * q->y - q->z * q->w);
			  rM.m[1][1] = 1.0f - 2.0f * (q->x * q->x + q->z * q->z);
			  rM.m[1][2] = 2.0f * (q->y * q->z + q->x * q->w);
			  rM.m[2][0] = 2.0f * (q->x * q->z + q->y * q->w);
			  rM.m[2][1] = 2.0f * (q->y * q->z - q->x * q->w);
			  rM.m[2][2] = 1.0f - 2.0f * (q->x * q->x + q->y * q->y);*/

			rM.m[0][0] = 1.0f - (2.0f * q->y * q->y) - (2.0f * q->z * q->z);
			rM.m[0][1] = (2.0f * q->x * q->y) + (2.0f * q->w * q->z);
			rM.m[0][2] = (2.0f * q->x * q->z) - (2.0f * q->w * q->y);
			rM.m[0][3] = 0.0f;

			rM.m[1][0] = (2.0f * q->x * q->y) - (2.0f * q->w * q->z);
			rM.m[1][1] = 1.0f - (2.0f * q->x * q->x) - (2.0f * q->z * q->z);
			rM.m[1][2] = (2.0f * q->y * q->z) + (2.0f * q->w * q->x);
			rM.m[1][3] = 0.0f;

			rM.m[2][0] = (2.0f * q->x * q->z) + (2.0f * q->w * q->y);
			rM.m[2][1] = (2.0f * q->y * q->z) - (2.0f * q->w * q->x);
			rM.m[2][2] = 1.0f - (2.0f * q->x * q->x) - (2.0f * q->y * q->y);
			rM.m[2][3] = 0.0f;

			rM.m[3][0] = 0.0f;
			rM.m[3][1] = 0.0f;
			rM.m[3][2] = 0.0f;
			rM.m[3][3] = 1.0f;

			return rM;
		}

		// ����Ɋ�Â��āA�E����W�n�p�[�X�y�N�e�B�u�ˉe�s����쐬����B
		Matrix MatrixPerspectiveFov(f32 fovY, f32 aspect, f32 zn, f32 zf)
		{
			Matrix rM;

			rM = MatrixIdentity();

			rM.m[0][0] = 1.0f / (aspect * TanF32(fovY / 2.0f));
			rM.m[1][1] = 1.0f / TanF32(fovY / 2.0f);
			rM.m[2][2] = zf / (zn - zf);
			rM.m[2][3] = -1.0f;
			rM.m[3][2] = (zf * zn) / (zn - zf);
			rM.m[3][3] = 0.0f;

			return rM;

			/*DirectX::XMMATRIX m = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, zn, zf);
			return ConvertToVector4x4FromMatrix(m);*/
		}

		// �J�X�^�}�C�Y�����E����W�n���ˉe�s����쐬����B
		Matrix MatrixOrthoOffCenter(f32 l, f32 r, f32 b, f32 t, f32 zn, f32 zf)
		{
			Matrix rM;

			rM = MatrixIdentity();

			rM.m[0][0] = 2.0f / (r - l);
			rM.m[1][1] = 2.0f / (t - b);
			rM.m[2][2] = 1.0f / (zn - zf);
			rM.m[3][0] = -1.0f - 2.0f * l / (r - l);
			rM.m[3][1] = 1.0f + 2.0f * t / (b - t);
			rM.m[3][2] = zn / (zn - zf);

			return rM;
		}

		// �E����W�n�r���[�s����쐬����B
		Matrix MatrixLookAt(const Vector3 eye, const Vector3 at, const Vector3 up)
		{
			Matrix rM;
			Vector3 x, y, z, t;

			t = Vector3Subtract(at, eye);
			z = Vector3Normalize(t);
			t = Vector3Cross(up, z);
			x = Vector3Normalize(t);
			y = Vector3Cross(z, x);

			rM.m[0][0] = -x.x;
			rM.m[1][0] = -x.y;
			rM.m[2][0] = -x.z;
			rM.m[3][0] = Vector3Dot(x, eye);
			rM.m[0][1] = y.x;
			rM.m[1][1] = y.y;
			rM.m[2][1] = y.z;
			rM.m[3][1] = -Vector3Dot(y, eye);
			rM.m[0][2] = -z.x;
			rM.m[1][2] = -z.y;
			rM.m[2][2] = -z.z;
			rM.m[3][2] = Vector3Dot(z, eye);
			rM.m[0][3] = 0.0f;
			rM.m[1][3] = 0.0f;
			rM.m[2][3] = 0.0f;
			rM.m[3][3] = 1.0f;

			return rM;

			/*DirectX::XMVECTOR E = DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(eye.x, eye.y, eye.z, 1.0f));
			DirectX::XMVECTOR F = DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(at.x, at.y, at.z, 1.0f));
			DirectX::XMVECTOR U = DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(up.x, up.y, up.z, 1.0f));

			DirectX::XMMATRIX m = DirectX::XMMatrixLookAtLH(E, F, U);
			return ConvertToVector4x4FromMatrix(m);*/
		}

		// �E����W�n���ˉe�s����쐬����B
		extern Matrix MatrixOrtho(f32 w, f32 h, f32 zn, f32 zf)
		{
			Matrix rM;
			rM = MatrixIdentity();

			rM.m[0][0] = 2.0f / w;
			rM.m[1][1] = 2.0f / h;
			rM.m[2][2] = 1.0f / (zn - zf);
			rM.m[3][2] = zn / (zn - zf);

			return rM;

			/*DirectX::XMMATRIX m = DirectX::XMMatrixOrthographicLH(w, h, zn, zf);
			return ConvertToVector4x4FromMatrix(m);*/
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

		inline Quaternion ConvertToQuaternionFromRotationMatrix(const Matrix m)
		{
			DirectX::XMVECTOR v;
			DirectX::XMMATRIX mT = ConvertToMatrixFromVector4x4(m);

			v = DirectX::XMQuaternionRotationMatrix(mT);

			return ConvertToQuaternionFromVector(v);
		}

		inline Matrix ConvertToRotationMatrixFromQuaternion(const Quaternion q)
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

		Quaternion QuaternionIdentity()
		{
			return ConvertToQuaternionFromVector(DirectX::XMQuaternionIdentity());
		}

		f32 QuaternionDot(const Quaternion q1, const Quaternion q2)
		{
			return (q1.x) * (q2.x) + (q1.y) * (q2.y) + (q1.z) * (q2.z) + (q1.w) * (q2.w);
		}

		Quaternion QuaternionMultiply(const Quaternion& q1, const Quaternion& q2)
		{
			Quaternion rq;

			DirectX::XMFLOAT4 v1 = DirectX::XMFLOAT4(q1.x, q1.y, q1.z, q1.w);
			DirectX::XMFLOAT4 v2 = DirectX::XMFLOAT4(q2.x, q2.y, q2.z, q2.w);
			DirectX::XMVECTOR vq1 = DirectX::XMLoadFloat4(&v1);
			DirectX::XMVECTOR vq2 = DirectX::XMLoadFloat4(&v2);
			DirectX::XMVECTOR rvq = DirectX::XMQuaternionMultiply(vq1, vq2);

			DirectX::XMFLOAT4 fq;
			DirectX::XMStoreFloat4(&fq, rvq);
			
			rq.x = fq.x;
			rq.y = fq.y;
			rq.z = fq.z;
			rq.w = fq.w;

			return rq;
		}

		Quaternion QuaternionRotationAxis(Vector3 axis, float angle)
		{
			Quaternion rq;

			DirectX::XMFLOAT4 v = DirectX::XMFLOAT4(axis.x, axis.y, axis.z, 1.0f);
			DirectX::XMVECTOR vq = DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat4(&v), angle);

			DirectX::XMFLOAT4 fq;
			DirectX::XMStoreFloat4(&fq, vq);

			rq.x = fq.x;
			rq.y = fq.y;
			rq.z = fq.z;
			rq.w = fq.w;

			return rq;
		}

		// Y������]���Ƃ��ăN�H�[�^�j�I������]������
		Quaternion QuaternionRotationY(f32 angle)
		{
			Quaternion rq;

			angle *= 0.5f;

			rq.x = 0;
			rq.y = SinF32(angle);
			rq.z = 0;
			rq.w = CosF32(angle);

			return rq;
		}

		Quaternion QuaternionSlerp(const Quaternion q1, const Quaternion q2, f32 t)
		{
			Quaternion qT;

			DirectX::XMFLOAT4 v1 = DirectX::XMFLOAT4(q1.x, q1.y, q1.z, q1.w);
			DirectX::XMFLOAT4 v2 = DirectX::XMFLOAT4(q2.x, q2.y, q2.z, q2.w);
			DirectX::XMVECTOR q1T = DirectX::XMLoadFloat4(&v1);
			DirectX::XMVECTOR q2T = DirectX::XMLoadFloat4(&v2);

			DirectX::XMVECTOR qV;
			qV = DirectX::XMQuaternionSlerp(q1T, q2T, t);
			DirectX::XMFLOAT4 qF;
			DirectX::XMStoreFloat4(&qF, qV);

			qT.x = qF.x;
			qT.y = qF.y;
			qT.z = qF.z;
			qT.w = qF.w;

			/*f32 epsilon = 1.0f;
			f32 dot = QuaternionDot(q1, q2);
			if (dot < 0.0f) epsilon = -1.0f;
			qT.x = (1.0f - t) * q1.x + epsilon * t * q2.x;
			qT.y = (1.0f - t) * q1.y + epsilon * t * q2.y;
			qT.z = (1.0f - t) * q1.z + epsilon * t * q2.z;
			qT.w = (1.0f - t) * q1.w + epsilon * t * q2.w;*/

			/*f32 epsilon = 1.0f;
			f32 dot = QuaternionDot(q1, q2);
			if (dot < 0.0f) epsilon = -1.0f;

			qT.x = (1.0f - t) * q1.x + epsilon * t * q2.x;
			qT.y = (1.0f - t) * q1.y + epsilon * t * q2.y;
			qT.z = (1.0f - t) * q1.z + epsilon * t * q2.z;
			qT.w = (1.0f - t) * q1.w + epsilon * t * q2.w;*/

			return qT;
		}
#pragma endregion
	} // namespace Math
} // namespace Phoenix