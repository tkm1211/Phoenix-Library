#include "pch.h"
#include "Phoenix/FND/STD.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/Math/Arithmetic.h"
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/Graphics/Camera.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// カメラ操作オブジェクト
		//****************************************************************************
		Camera::Camera()
		{
			SetPerspective(Math::PI_Q, 16.0f / 9.0f, 0.1f, 1000.0f);
			SetLookAt(Math::Vector3::OneAll, Math::Vector3::Zero, Math::Vector3::OneY);
			Update();
		}

		// ビュータイプ設定
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


		// 更新
		void Camera::Update()
		{
			// ビュー
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

			// プロジェクション
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

			// 視錐体
			/*Math::Matrix view_projection;
			Math::MatrixMultiply(&view_projection, &m_view, &m_projection);
			m_frustum.InitFromMatrix(view_projection);*/
		}

		// ビュープロジェクション設定
		void Camera::SetViewProjection(const Math::Matrix& view, const Math::Matrix& projection)
		{
			this->view = view;
			this->projection = projection;

			// ビュー
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

			// プロジェクション
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

			// 視錐体
			/*Math::Matrix viewProjection;
			viewProjection = Math::MatrixMultiply(view, projection);
			frustum.InitFromMatrix(viewProjection);*/
		}


		// Y軸画角設定
		void Camera::SetFovY(f32 fovY)
		{
			this->fovY = fovY;
			SetViewType(!IsOrtho());
		}

		// アスペクト比設定
		void Camera::SetAspect(f32 aspect)
		{
			this->aspect = aspect;
			SetViewType(!IsOrtho());
		}

		// ニアクリップ値設定
		void Camera::SetNearZ(f32 nearZ)
		{
			this->nearZ = nearZ;
			SetViewType(!IsOrtho());
		}

		// ファークリップ値設定
		void Camera::SetFarZ(f32 farZ)
		{
			this->farZ = farZ;
			SetViewType(!IsOrtho());
		}

		// 透視投影行列設定
		void Camera::SetPerspective(f32 fovY, f32 aspect, f32 nearZ, f32 farZ)
		{
			projection = Math::MatrixPerspectiveFov(fovY, aspect, nearZ, farZ);
		}

		// 並行投影行列設定
		void Camera::SetOrthographic(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ)
		{
			projection = Math::MatrixOrthoOffCenter(left, right, bottom, top, nearZ, farZ);
		}

		// ビュー行列設定
		void Camera::SetLookAt(const Math::Vector3& eye, const Math::Vector3& focus, const Math::Vector3& up)
		{
			view = Math::MatrixLookAt(eye, focus, up);
			lookAtDistance = Math::Vector3Length(focus - eye);
		}

		// ビュー行列を設定
		void Camera::SetView(const Math::Matrix& view)
		{
			this->view = view;
		}

		// プロジェクション行列を設定
		void Camera::SetProjection(const Math::Matrix& projection)
		{
			this->projection = projection;
		}

		// スクリーン用ビュープロジェクション行列設定
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

		// ビルボード行列を計算
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

		// ビューベクトルを取得
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

		// 視錐体を設定
		/*void Camera::SetFructum(const Math::Matrix& view, const Math::Matrix& projection)
		{
			Math::Matrix view_projection;
			Math::MatrixMultiply(&view_projection, &view, &projection);
			m_frustum.InitFromMatrix(view_projection);
		}*/

		// 球の範囲が視界に入るようにズーム
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
	} // namespace Graphics
} // namespace Phoenix