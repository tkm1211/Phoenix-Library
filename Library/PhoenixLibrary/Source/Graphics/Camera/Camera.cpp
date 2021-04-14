#include "pch.h"
#include "Phoenix/FND/STD.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/Math/Arithmetic.h"
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �J��������I�u�W�F�N�g
		//****************************************************************************
		Camera::Camera()
		{
			SetPerspective(30.0f * 0.01745f, 16.0f / 9.0f, 0.1f, 100000.0f);
			SetLookAt(Math::Vector3::OneAll, Math::Vector3::Zero, Math::Vector3::OneY);
			Update();

			rotateX = 0.5f;
			rotateY = 0.0f; //-3.14f
			distance = 5.0f;
		}

		// �r���[�^�C�v�ݒ�
		void Camera::SetViewType(ViewType viewType)
		{
			if (this->viewType != viewType)
			{
				this->viewType = viewType;

				if (this->viewType == ViewType::Perspective)
				{
					SetPerspective(fovY, aspect, nearZ, farZ);
				}
				else
				{
					Math::Vector3 front, up;
					GetViewVectors(front, up);

					f32 d = lookAtDistance;
					SetLookAt(focus - front * d, focus, up);
					SetOrthographic(
						d * aspect / 2,
						-d * aspect / 2,
						d / 2,
						-d / 2,
						nearZ,
						farZ);
				}
			}
		}

		void Camera::SetViewType(bool perspective)
		{
			if (perspective)
			{
				viewType = ViewType::Perspective;
				SetPerspective(fovY, aspect, nearZ, farZ);
			}
			else
			{
				viewType = ViewType::Orthographic;
				f32 d = lookAtDistance;
				SetOrthographic(
					d * aspect / 2,
					-d * aspect / 2,
					d / 2,
					-d / 2,
					nearZ,
					farZ);
			}
		}


		// �X�V
		void Camera::Update()
		{
			// �r���[
			Math::Matrix inverse;
			inverse = Math::MatrixInverse(view);

			right.x = inverse.m[0][0];
			right.y = inverse.m[0][1];
			right.z = inverse.m[0][2];

			up.x = inverse.m[1][0];
			up.y = inverse.m[1][1];
			up.z = inverse.m[1][2];

			front.x = -inverse.m[2][0];
			front.y = -inverse.m[2][1];
			front.z = -inverse.m[2][2];

			eye.x = inverse.m[3][0];
			eye.y = inverse.m[3][1];
			eye.z = inverse.m[3][2];

			focus.x = eye.x + front.x * lookAtDistance;
			focus.y = eye.y + front.y * lookAtDistance;
			focus.z = eye.z + front.z * lookAtDistance;

			// �v���W�F�N�V����
			Math::Matrix inverseProjection;
			inverseProjection = Math::MatrixInverse(projection);

			Math::Vector3 p0, p1;
			p0 = Math::Vector3TransformCoord(Math::Vector3::Zero, inverseProjection);
			p1 = Math::Vector3TransformCoord(Math::Vector3::OneZ, inverseProjection);

			if (!IsOrtho())
			{
				fovY = 2.0f * Math::ATanF32(1.0f / projection.m[1][1]);
			}
			aspect = projection.m[1][1] / projection.m[0][0];
			nearZ = FND::FAbs(p0.z);
			farZ = FND::FAbs(p1.z);

			// ������
			/*Math::Matrix view_projection;
			Math::MatrixMultiply(&view_projection, &m_view, &m_projection);
			m_frustum.InitFromMatrix(view_projection);*/
		}

		// �r���[�v���W�F�N�V�����ݒ�
		void Camera::SetViewProjection(const Math::Matrix& view, const Math::Matrix& projection)
		{
			this->view = view;
			this->projection = projection;

			// �r���[
			Math::Matrix inverseView;
			inverseView = Math::MatrixInverse(view);

			right.x = inverseView.m[0][0];
			right.y = inverseView.m[0][1];
			right.z = inverseView.m[0][2];

			up.x = inverseView.m[1][0];
			up.y = inverseView.m[1][1];
			up.z = inverseView.m[1][2];

			front.x = -inverseView.m[2][0];
			front.y = -inverseView.m[2][1];
			front.z = -inverseView.m[2][2];

			eye.x = inverseView.m[3][0];
			eye.y = inverseView.m[3][1];
			eye.z = inverseView.m[3][2];

			focus.x = eye.x + front.x * lookAtDistance;
			focus.y = eye.y + front.y * lookAtDistance;
			focus.z = eye.z + front.z * lookAtDistance;

			// �v���W�F�N�V����
			Math::Matrix inverseProjection;
			inverseProjection = Math::MatrixInverse(projection);

			Math::Vector3 p0, p1;
			p0 = Math::Vector3TransformCoord(Math::Vector3::Zero, inverseProjection);
			p1 = Math::Vector3TransformCoord(Math::Vector3::OneZ, inverseProjection);

			if (!IsOrtho())
			{
				fovY = 2.0f * Math::ATanF32(1.0f / projection.m[1][1]);
			}
			aspect = projection.m[1][1] / projection.m[0][0];
			nearZ = FND::FAbs(p0.z);
			farZ = FND::FAbs(p1.z);

			// ������
			/*Math::Matrix viewProjection;
			viewProjection = Math::MatrixMultiply(view, projection);
			frustum.InitFromMatrix(viewProjection);*/
		}


		// Y����p�ݒ�
		void Camera::SetFovY(f32 fovY)
		{
			this->fovY = fovY;
			SetViewType(!IsOrtho());
		}

		// �A�X�y�N�g��ݒ�
		void Camera::SetAspect(f32 aspect)
		{
			this->aspect = aspect;
			SetViewType(!IsOrtho());
		}

		// �j�A�N���b�v�l�ݒ�
		void Camera::SetNearZ(f32 nearZ)
		{
			this->nearZ = nearZ;
			SetViewType(!IsOrtho());
		}

		// �t�@�[�N���b�v�l�ݒ�
		void Camera::SetFarZ(f32 farZ)
		{
			this->farZ = farZ;
			SetViewType(!IsOrtho());
		}

		// �������e�s��ݒ�
		void Camera::SetPerspective(f32 fovY, f32 aspect, f32 nearZ, f32 farZ)
		{
			projection = Math::MatrixPerspectiveFov(fovY, aspect, nearZ, farZ);
		}

		// ���s���e�s��ݒ�
		void Camera::SetOrthographic(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ)
		{
			projection = Math::MatrixOrthoOffCenter(left, right, bottom, top, nearZ, farZ);
		}

		// �r���[�s��ݒ�
		void Camera::SetLookAt(const Math::Vector3& eye, const Math::Vector3& focus, const Math::Vector3& up)
		{
			view = Math::MatrixLookAt(eye, focus, up);
			lookAtDistance = Math::Vector3Length(focus - eye);
		}

		// �r���[�s���ݒ�
		void Camera::SetView(const Math::Matrix& view)
		{
			this->view = view;
		}

		// �v���W�F�N�V�����s���ݒ�
		void Camera::SetProjection(const Math::Matrix& projection)
		{
			this->projection = projection;
		}

		// �X�N���[���p�r���[�v���W�F�N�V�����s��ݒ�
		void Camera::SetScreen(f32 width, f32 height)
		{
			aspect = width / height;

			view = Math::MatrixIdentity();
			view.m[2][2] = -1.0f;
			view.m[3][0] = -width * 0.5f;
			view.m[3][1] = -height * 0.5f;

			projection = Math::MatrixOrtho(width, -height, 0, 1);
		}

		// compute unit per pixel at the given world position.
		f32 Camera::ComputeUnitPerPixel(const Math::Vector3& worldPosition, f32 vh) const
		{
			f32 worldWidth, worldHeight;
			ComputeWorldDimensions(worldPosition, worldWidth, worldHeight);
			return (worldHeight / vh);
		}

		// compute world dimensions at the given world position.
		void Camera::ComputeWorldDimensions(const Math::Vector3& worldPosition, f32& width, f32& height) const
		{
			if (IsOrtho())
			{
				width = 2.0f / projection._11;
				height = 2.0f / projection._22;
			}
			else
			{
				// Transform worldPos to view space.
				Math::Vector3 v;
				v = Math::Vector3TransformCoord(worldPosition, view);
				f32  dist = FND::FAbs(v.z);
				width = (2.0f * dist) / projection._11;
				height = (2.0f * dist) / projection._22;
			}
		}

		// �r���{�[�h�s����v�Z
		void Camera::ComputeBillboard(const Math::Vector3& worldPosition, Math::Matrix& billboard) const
		{
			billboard._11 = view._11;
			billboard._12 = view._21;
			billboard._13 = view._31;
			billboard._14 = 0.0f;
			billboard._21 = view._12;
			billboard._22 = view._22;
			billboard._23 = view._32;
			billboard._24 = 0.0f;
			billboard._31 = view._13;
			billboard._32 = view._23;
			billboard._33 = view._33;
			billboard._34 = 0.0f;
			billboard._41 = worldPosition.x;
			billboard._42 = worldPosition.y;
			billboard._43 = worldPosition.z;
			billboard._44 = 1.0f;
		}

		// �r���[�x�N�g�����擾
		void Camera::GetViewVectors(Math::Vector3& front, Math::Vector3& up)
		{
			switch (viewType)
			{
			case ViewType::Front:
				front = Math::Vector3(0, 0, -1);
				up = Math::Vector3(0, 1, 0);
				break;

			case ViewType::Back:
				front = Math::Vector3(0, 0, 1);
				up = Math::Vector3(0, 1, 0);
				break;

			case ViewType::Top:
				front = Math::Vector3(0, -1, 0);
				up = Math::Vector3(0, 0, -1);
				break;

			case ViewType::Bottom:
				front = Math::Vector3(0, 1, 0);
				up = Math::Vector3(0, 0, -1);
				break;

			case ViewType::Left:
				front = Math::Vector3(1, 0, 0);
				up = Math::Vector3(0, 1, 0);
				break;

			case ViewType::Right:
				front = Math::Vector3(-1, 0, 0);
				up = Math::Vector3(0, 1, 0);
				break;

			case ViewType::Perspective:
				front = this->front;
				up = this->up;
				break;
			}
		}

		// �����̂�ݒ�
		/*void Camera::SetFructum(const Math::Matrix& view, const Math::Matrix& projection)
		{
			Math::Matrix view_projection;
			Math::MatrixMultiply(&view_projection, &view, &projection);
			m_frustum.InitFromMatrix(view_projection);
		}*/

		// ���͈̔͂����E�ɓ���悤�ɃY�[��
		void Camera::ZoomOnSphere(const Math::Vector3& center, f32 radius)
		{
			f32 d = radius;
			if (d == 0.0f)
			{
				d = 1;
			}

			if (d > farZ)
			{
				d = farZ;
			}
			else if (d < nearZ)
			{
				d = nearZ;
			}

			focusRadius = d;

			Math::Vector3 front, up;
			GetViewVectors(front, up);

			Math::Vector3 eye = center - front * d;
			SetLookAt(eye, center, up);

#if 0
			if (m_view_type == ViewType::Perspective)
			{
				f32 near_z = fnd::FAbs(radius) * 0.1f;
				near_z = fnd::Max(near_z, 0.001f);
				m_near_z = near_z;
			}
#endif
		}

		// �t���[�J����
		void Camera::FreeCamera()
		{
			// TODO : Win�֐���ʂ̊֐��ɍ����ւ�
			POINT cursor;
			GetCursorPos(&cursor);

			oldCursor = newCursor;
			newCursor = Math::Vector2(static_cast<float>(cursor.x), static_cast<float>(cursor.y));

			f32 moveX = (newCursor.x - oldCursor.x) * 0.02f;
			f32 moveY = (newCursor.y - oldCursor.y) * 0.02f;

			//if (GetKeyState(VK_MENU) < 0)
			//{
			//	if (GetKeyState(VK_LBUTTON) < 0)
			//	{
			//		rotateY -= moveX * 0.5f;
			//		rotateX += moveY * 0.5f;
			//	}
			//	else if (GetKeyState(VK_MBUTTON) < 0)
			//	{
			//		float s = distance * 0.035f;
			//		float x = moveX * s;
			//		float y = moveY * s;

			//		target.x -= right.x * x;
			//		target.y -= right.y * x;
			//		target.z -= right.z * x;

			//		target.x += up.x * y;
			//		target.y += up.y * y;
			//		target.z += up.z * y;
			//	}
			//	else if (GetKeyState(VK_RBUTTON) < 0)
			//	{
			//		distance += (-moveY - moveX) * distance * 0.1f;
			//	}
			//	else
			//	{
			//		//distance -= (float)MOUSE.diMouseState.lZ / 1.0f;
			//	}

			//	f32 xSin = sinf(rotateX);
			//	f32 xCos = cosf(rotateX);
			//	f32 ySin = sinf(rotateY);
			//	f32 yCos = cosf(rotateY);

			//	Math::Vector3 front = { -xCos * ySin, -xSin, -xCos * yCos };
			//	Math::Vector3 _right = { yCos, 0.0f, -ySin };
			//	Math::Vector3 _up = Math::Vector3Cross(_right, front);

			//	Math::Vector3 _target = target;
			//	Math::Vector3 _distance = { distance, distance, distance };
			//	Math::Vector3 _pos = _target - (front * _distance);

			//	SetLookAt(_pos, _target, _up);
			//}
			//else 
			if (GetKeyState(VK_RBUTTON) < 0)
			{
				f32 xSin = sinf(rotateX);
				f32 xCos = cosf(rotateX);
				f32 ySin = sinf(rotateY);
				f32 yCos = cosf(rotateY);

				Math::Vector3 front = { xCos * -ySin, xSin, xCos * -yCos };
				Math::Vector3 _right = { yCos, 0.0f, -ySin };
				Math::Vector3 _up = Math::Vector3Cross(_right, front);

				rotateY -= moveX * 0.25f;
				rotateX += moveY * 0.25f;

				if (GetKeyState('W') < 0)
				{
					eye += -front * speed;
				}
				if (GetKeyState('S') < 0)
				{
					eye += front * speed;
				}
				if (GetKeyState('A') < 0)
				{
					eye += _right * speed;
				}
				if (GetKeyState('D') < 0)
				{
					eye += -_right * speed;
				}
				if (GetKeyState('Q') < 0)
				{
					eye += -_up * speed;
				}
				if (GetKeyState('E') < 0)
				{
					eye += _up * speed;
				}

				xSin = sinf(rotateX);
				xCos = cosf(rotateX);
				ySin = sinf(rotateY);
				yCos = cosf(rotateY);

				front = { xCos * -ySin, xSin, xCos * -yCos };
				_right = { yCos, 0.0f, -ySin };
				_up = Math::Vector3Cross(_right, front);

				Math::Vector3 _pos = eye;
				Math::Vector3 _distance = { distance, distance, distance };
				//Math::Vector3 _pos = _target - (front * _distance);
				Math::Vector3 _target = _pos - (front * _distance);
				//target = _target;

				SetLookAt(_pos, _target, _up);
			}
		}

		void Camera::SurveyCamera(f32 addRotateX, f32 addRotateY, f32 distance, Math::Vector3 target)
		{
			rotateX += addRotateX;
			rotateY += addRotateY;

			f32 xSin = sinf(rotateX);
			f32 xCos = cosf(rotateX);
			f32 ySin = sinf(rotateY);
			f32 yCos = cosf(rotateY);

			Math::Vector3 front = { -xCos * ySin, -xSin, -xCos * yCos };
			Math::Vector3 _right = { yCos, 0.0f, -ySin };
			Math::Vector3 _up = Math::Vector3Cross(_right, front);

			Math::Vector3 _target = target;
			Math::Vector3 _distance = { distance, distance, distance };
			Math::Vector3 _pos = _target - (front * _distance);

			SetLookAt(_pos, _target, _up);
		}

		void Camera::ControllerCamera(const Math::Vector3& center, const Math::Vector3& adjust, const Phoenix::f32 lerpTime)
		{
			// TODO : Win�֐���ʂ̊֐��ɍ����ւ�
			POINT cursor;
			GetCursorPos(&cursor);
			oldCursor = newCursor;
			newCursor = Math::Vector2(static_cast<float>(cursor.x), static_cast<float>(cursor.y));

			Phoenix::f32 sX = 0.0f;
			Phoenix::f32 sY = 0.0f;

			sX = xInput[0].sRX / 1000.0f;
			sY = xInput[0].sRY / 1000.0f;

			sY = GetKeyState(VK_UP) < 0 ? -1.0f : sY;
			sY = GetKeyState(VK_DOWN) < 0 ? 1.0f : sY;
			sX = GetKeyState(VK_LEFT) < 0 ? -1.0f : sX;
			sX = GetKeyState(VK_RIGHT) < 0 ? 1.0f : sX;

			if (GetKeyState(VK_RBUTTON) < 0)
			{
				f32 moveX = (newCursor.x - oldCursor.x) * 0.02f;
				f32 moveY = (newCursor.y - oldCursor.y) * 0.02f;
				rotateY -= moveX * 0.5f;
				rotateX += moveY * 0.5f;
			}
			else
			{
				rotateY -= sX * 3.5f * 0.01745f;
				rotateX -= sY * 3.5f * 0.01745f;
				if (0.0f < rotateX)
				{
					rotateX = 0.0f;
				}
				if (rotateX < -1.0f)
				{
					rotateX = -1.0f;
				}
			}

			f32 xSin = sinf(rotateX);
			f32 xCos = cosf(rotateX);
			f32 ySin = sinf(rotateY);
			f32 yCos = cosf(rotateY);

			// ���̃t���[���̃x�N�g�����g�p����̂ł����Ōv�Z���Ă���
			Math::Vector3 _front = { xCos * -ySin, xSin, xCos * -yCos };
			Math::Vector3 _right = { yCos, 0.0f, -ySin };
			Math::Vector3 _up = Math::Vector3Cross(_right, _front);
			front = _front;

			// �����Œ��S���v�Z���Ă������ƂŒ����_�ƃJ�������W�����߂邱�Ƃ��ł���
			//eye = (center + adjust);
			eye = Phoenix::Math::Vector3Lerp(eye, (center + adjust), lerpTime);

			// �����_���^�[�Q�b�g�Ɉړ����Ă��鎞�ƈړ����Ă��Ȃ����ŕ��򂵂Ă������Ƃŏ��������₷���Ȃ�
			if (onTarget)
			{
				// ��ʂɃ^�[�Q�b�g���f���Ă��Ȃ���Ԃ��炢���Ȃ�f��ƈ�a�����o��
				// �����Ő��`��Ԃ��g�p���āA���X�ɒ����_���^�[�Q�b�g�ɋ߂Â��Ă������ƂŐU��������悤�ɂȂ�
				focus = Phoenix::Math::Vector3Lerp(focus, targetPos, 0.1f);

				// ������x�A�߂Â��Ă�����t���[�J�����ɖ߂�������
				f32 dis = fabs(Phoenix::Math::Vector3Length(focus - targetPos));
				if (dis <= 1.0f)
				{
					targetCnt = 0;
					onTarget = false;
				}

				// ���̃t���[���ŉ�]�����l�����̃t���[���Ŏg�p����̂ł����Ōv�Z���Ă���
				Math::Vector3 dir = center - focus;
				dir = Math::Vector3Normalize(dir);
				rotateX = atan2f(-dir.y, dir.z);
				rotateY = atan2f(dir.x, dir.z);
			}
			else
			{
				focus = Phoenix::Math::Vector3Lerp(focus, (eye - (front * -7.5f)), 0.5f);
			}

			// �J�������W���v�Z���鎞�ɁA�O�x�N�g�����ς���Ă��܂��Ă���̂ōČv�Z
			front = Phoenix::Math::Vector3Normalize(focus - eye);

			Math::Vector3 _pos = eye - (front * 7.5f);
			_pos.y = _pos.y <= 1.0f ? 1.0f : _pos.y; // �X�e�[�W�ɖ��܂��Ă��܂��̂ŉ����o��
		
			// �J�����V�F�C�N�l�𒍎��_�ɉ��Z����ƃY����������̂ŕϐ��ɑ��
			Math::Vector3 _target = focus + shake;

			_right = Math::Vector3Cross(front, _up);
			_up = Math::Vector3Cross(_right, front);

			SetLookAt(_pos, _target, _up);
		}

		void Camera::ControllerCamera02(bool onControl, const Math::Vector3& center, const Math::Vector3& adjust, const Phoenix::f32 len, const Phoenix::f32 elapsedTime, const Phoenix::f32 lerpTime, bool adjustRotate, const Phoenix::Math::Vector3 targetFrontVec)
		{
			// TODO : Win�֐���ʂ̊֐��ɍ����ւ�
			POINT cursor;
			GetCursorPos(&cursor);
			oldCursor = newCursor;
			newCursor = Math::Vector2(static_cast<float>(cursor.x), static_cast<float>(cursor.y));

			Phoenix::f32 sX = 0.0f;
			Phoenix::f32 sY = 0.0f;

			sX = xInput[0].sRX / 1000.0f;
			sY = xInput[0].sRY / 1000.0f;

			sY = GetKeyState(VK_UP) < 0 ? -1.0f : sY;
			sY = GetKeyState(VK_DOWN) < 0 ? 1.0f : sY;
			sX = GetKeyState(VK_LEFT) < 0 ? -1.0f : sX;
			sX = GetKeyState(VK_RIGHT) < 0 ? 1.0f : sX;

			if (onControl)
			{
				if (GetKeyState(VK_RBUTTON) < 0)
				{
					f32 moveX = (newCursor.x - oldCursor.x) * 0.02f;
					f32 moveY = (newCursor.y - oldCursor.y) * 0.02f;
					rotateY -= moveX * 0.5f * elapsedTime;
					//rotateX += moveY * 0.5f * elapsedTime;
				}
				else
				{
					rotateY -= sX * 3.5f * 0.01745f * elapsedTime;
					/*rotateX -= sY * 3.5f * 0.01745f * elapsedTime;
					if (0.5f < rotateX)
					{
						rotateX = 0.5f;
					}
					if (rotateX < -0.5f)
					{
						rotateX = -0.5f;
					}*/
				}
			}

			//if (sY == 0.0f)
			{
				//rotateX = Phoenix::Math::f32Lerp(rotateX, -0.225f, 0.15f * elapsedTime);
				rotateX = -0.225f;
			}

			f32 xSin = sinf(rotateX);
			f32 xCos = cosf(rotateX);
			f32 ySin = sinf(rotateY);
			f32 yCos = cosf(rotateY);

			// ���̃t���[���̃x�N�g�����g�p����̂ł����Ōv�Z���Ă���
			Math::Vector3 _front = { xCos * -ySin, xSin, xCos * -yCos };
			Math::Vector3 _right = { yCos, 0.0f, -ySin };
			Math::Vector3 _up = Math::Vector3Cross(_right, _front);
			front = _front;

			// �΂߂ɕ��
			bool hit = false;
			static bool oldHit = false;
			if (sX == 0.0f && adjustRotate && (targetFrontVec.x != 0.0f || targetFrontVec.z != 0.0f))
			{
				_front.y = 0.0f;

				Phoenix::f32 dot = Phoenix::Math::Vector3Dot(targetFrontVec, _front);
				Phoenix::f32 angle = acosf(dot) / 0.01745f;

				if (1e-8f < fabs(angle))
				{
					Phoenix::f32 angleR;
					angleR = acosf(Phoenix::Math::Vector3Dot(targetFrontVec, _right));
					angleR -= (90.0f * 0.01745f);

					if (0.0f < angleR)
					{
						angle *= -1;
					}

					if (0.0f <= fabs(angle) && fabs(angle) < 35.0f)
					{
						hit = true;

						Phoenix::f32 originAngle = rotateY;
						Phoenix::f32 newAngle = originAngle + (angle * 0.01745f);

						if (!oldHit && 25.0f <= fabs(angle) && fabs(angle) < 35.0f)
						{
							adjustAngle = rotateY;
						}
						else
						{
							adjustAngle = Phoenix::Math::f32Lerp(adjustAngle, newAngle, lerpTime * elapsedTime);
							rotateY = adjustAngle;
							oldHit = hit;
						}
					}
				}
			}
			if (!hit)
			{
				adjustAngle = rotateY;
				oldHit = false;
			}

			// �����_���ς���Ă����`��Ԃň�a�����Ȃ���
			focus = Phoenix::Math::Vector3Lerp(focus, (center + adjust), lerpTime * elapsedTime);

			Math::Vector3 _pos = focus - (front * len);
			_pos.y = _pos.y <= 1.0f ? 1.0f : _pos.y; // �X�e�[�W�ɖ��܂��Ă��܂��̂ŉ����o��
			eye = _pos;

			// �J�����V�F�C�N�l�𒍎��_�ɉ��Z����ƃY����������̂ŕϐ��ɑ��
			Math::Vector3 _target = focus + shake;

			_right = Math::Vector3Cross(front, _up);
			_up = Math::Vector3Cross(_right, front);

			SetLookAt(_pos + shake, _target, _up);
		}

		void Camera::LockOnCamera(const Math::Vector3& center, const Math::Vector3& target, const Math::Vector3& centerAdjust, const Math::Vector3& targetAdjust, bool isLerp)
		{
#if 0
			Math::Vector3 cameraPos;

			Math::Vector3 dir = pos - center;
			dir.y = 0.0f;

			f32 len = Math::Vector3Length(dir) + 400.0f;
			Math::Vector3 dirN = Math::Vector3Normalize(dir);

			rotateY = atan2f(dirN.x, dirN.z);
			cameraPos = center + dirN * len;

			Math::Vector3 right = Math::Vector3Cross(Math::Vector3::OneY, -dirN);
			cameraPos += right * -150.0f;
			cameraPos.y = pos.y;

			SetLookAt(cameraPos, center, Math::Vector3::OneY);
#elif 0
			Math::Vector3 dir = (center + centerAdjust) - (target + targetAdjust);
			dir.y = center.y - target.y /*0.0f*/;
			dir = Math::Vector3Normalize(dir);

			rotateX = 0.0f;
			rotateY = atan2f(dir.x, dir.z);

			eye = (center + centerAdjust); /*Phoenix::Math::Vector3Lerp(eye, center, 0.6f)*/
			focus = Phoenix::Math::Vector3Lerp(focus, (target + targetAdjust), 0.05f);
			front = Phoenix::Math::Vector3Lerp(front, -dir, 0.05f);

			Math::Vector3 _pos = eye - (front * 6.5f);
			Math::Vector3 _target = focus;

			dir = center - target;
			dir.y = 0.0f;
			Math::Vector3 dirN = Math::Vector3Normalize(dir);
			Math::Vector3 right = Math::Vector3Cross(Math::Vector3::OneY, -dirN);
			_pos += right * -0.75f;

			_pos.y = _pos.y <= 1.0f ? 1.0f : _pos.y;

			SetLookAt(_pos, _target, Math::Vector3::OneY);
#elif 1
			eye = (center + centerAdjust); /*Phoenix::Math::Vector3Lerp(eye, center, 0.6f)*/
			if (isLerp) focus = Phoenix::Math::Vector3Lerp(focus, (target + targetAdjust), 0.05f);
			else focus = target + targetAdjust;

			Math::Vector3 dir = eye - focus;
			dir.y = center.y - target.y /*0.0f*/;
			dir = Math::Vector3Normalize(dir);

			rotateX = 0.0f;
			rotateY = atan2f(dir.x, dir.z);

			front = Phoenix::Math::Vector3Lerp(front, -dir, 0.05f);

			Math::Vector3 _pos = eye - (front * 6.5f);
			Math::Vector3 _target = focus + shake;

			dir = center - target;
			dir.y = 0.0f;
			Math::Vector3 dirN = Math::Vector3Normalize(dir);

			right = Math::Vector3Cross(Math::Vector3::OneY, -dirN);
			_pos += right * -adjustRight;

			//Phoenix::Math::Vector3 vec = Vector3SphereLinear(right, front, 0.5f);
			//_pos += vec * -adjustRight;

			_pos.y = _pos.y <= 1.0f ? 1.0f : _pos.y;

			SetLookAt(_pos, _target, Math::Vector3::OneY);
#endif
		}

		void Camera::SetTargetPos(const Math::Vector3& target, const Math::Vector3& targetAdjust)
		{
			onTarget = true;
			targetPos = target + targetAdjust;
			targetCnt = 0;
		}

		void Camera::SphereLinearLockOnCamera(const Math::Vector3& center, const Math::Vector3& start, const Math::Vector3& end, const Math::Vector3& centerAdjust, f32 sphereLinearSpeed, f32 distanceToFouceFromCamera)
		{
			eye = (center + centerAdjust); /*Phoenix::Math::Vector3Lerp(eye, center, 0.6f)*/
			Math::Vector3 startVec = start - eye;
			Math::Vector3 endVec = end - eye;

			startVec.y = 0.0f;
			endVec.y = 0.0f;

			Math::Vector3 vec = Phoenix::Math::Vector3SphereLinear(start, end, sphereLinearSpeed);
			focus = eye + vec * distanceToFouceFromCamera;

			Math::Vector3 dir = eye - focus;
			dir.y = center.y - target.y /*0.0f*/;
			dir = Math::Vector3Normalize(dir);

			rotateX = 0.0f;
			rotateY = atan2f(dir.x, dir.z);

			front = Phoenix::Math::Vector3Lerp(front, -dir, 0.05f);

			Math::Vector3 _pos = eye - (front * 6.5f);
			Math::Vector3 _target = focus;

			dir = center - target;
			dir.y = 0.0f;
			Math::Vector3 dirN = Math::Vector3Normalize(dir);
			Math::Vector3 right = Math::Vector3Cross(Math::Vector3::OneY, -dirN);
			_pos += right * -0.75f;

			_pos.y = _pos.y <= 1.0f ? 1.0f : _pos.y;

			SetLookAt(_pos, _target, Math::Vector3::OneY);
		}

		void Camera::InitEventCamera(const Math::Vector3& focus, const Math::Vector3& front, Phoenix::f32 len)
		{
			this->focus = focus;

			Math::Vector3 _target = focus + shake;
			Math::Vector3 _right = Math::Vector3Cross(front, Math::Vector3::OneY);
			Math::Vector3 _up = Math::Vector3Cross(_right, front);
			Math::Vector3 _pos = this->focus - (front * len);

			eye = _pos;

			SetLookAt(_pos, _target, _up);
		}

		void Camera::EventCamera(const Math::Vector3& focus, const Math::Vector3& front, Phoenix::f32 len)
		{
			this->focus = Phoenix::Math::Vector3Lerp(this->focus, focus, 0.5f);
			
			Math::Vector3 _target = focus + shake;
			Math::Vector3 _right = Math::Vector3Cross(front, Math::Vector3::OneY);
			Math::Vector3 _up = Math::Vector3Cross(_right, front);
			Math::Vector3 _pos = this->focus - (front * len);

			eye = _pos;

			SetLookAt(_pos, _target, _up);
		}
	} // namespace Graphics
} // namespace Phoenix