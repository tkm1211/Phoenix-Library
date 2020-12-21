#pragma once

#include "Phoenix/FND/NonCopyable.h"
//#include "Phoenix/Math/Frustum.h"
#include "Phoenix/Math/Vector.h"
#include "Phoenix/Math/Matrix.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �r���[�^�C�v
		//****************************************************************************
		enum class ViewType
		{
			Front,				// ���s���e�E�O
			Back,				// ���s���e�E��
			Top,				// ���s���e�E��
			Bottom,				// ���s���e�E��
			Left,				// ���s���e�E��
			Right,				// ���s���e�E�E
			Perspective,		// �������e
			Orthographic,		// ���s���e

			TypeNum
		};

		//****************************************************************************
		// �J��������I�u�W�F�N�g
		//****************************************************************************
		class Camera final : public FND::NonCopyable
		{
		private:
			ViewType viewType = ViewType::Perspective;

			Math::Matrix projection;
			Math::Matrix view;
			Math::Matrix inverseView;
			//Math::Frustum	frustum;
			Math::Vector3 eye;
			Math::Vector3 focus;
			Math::Vector3 front;
			Math::Vector3 up;
			Math::Vector3 right;
			f32 lookAtDistance;
			f32 focusRadius;
			f32 aspect;
			f32 fovY;
			f32 nearZ;
			f32 farZ;

			Math::Vector3 target;
			Math::Vector2 oldCursor;
			Math::Vector2 newCursor;
			f32 rotateX;
			f32 rotateY;
			float distance;

			Phoenix::Math::Vector3 shake = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);

			f32 adjustAngle = 0.0f;

		public:
			f32 speed = 1.0f;
			f32 adjustRight = 0.75f;

			bool onTarget = false;
			s32 targetCnt = 0;
			s32 targetMaxCnt = 10;
			Math::Vector3 targetPos = { 0.0f, 0.0f, 0.0f };

		public:
			Camera();
			~Camera() {}

		public:
			// �r���[�^�C�v�ݒ�
			void SetViewType(ViewType viewType);
			void SetViewType(bool perspective);

			// �r���[�^�C�v�擾
			ViewType GetViewType() const { return viewType; }

			// �X�V
			void Update();

			// �r���[�v���W�F�N�V�����ݒ�
			void SetViewProjection(const Math::Matrix& view, const Math::Matrix& projection);

			// �����̎擾
			//const Math::Frustum& GetFrustum() const { return m_frustum; }

			// �v���W�F�N�V�����s��擾
			const Math::Matrix& GetProjection() const { return projection; }

			// �r���[�s��擾
			const Math::Matrix& GetView() const { return view; }

			// ���_�ʒu�擾
			const Math::Vector3& GetEye() const { return eye; }

			// �����_�ʒu�擾
			const Math::Vector3& GetFocus() const { return focus; }

			// �O�x�N�g���擾
			const Math::Vector3& GetFront() const { return front; }

			// ��x�N�g���擾
			const Math::Vector3& GetUp() const { return up; }

			// �E�x�N�g���擾
			const Math::Vector3& GetRight() const { return right; }

			// ���_���璍���_�̋����擾
			f32 GetDistanceFromLookAt() const { return lookAtDistance; }

			// ���_�𒆐S�Ƃ����t�H�[�J�X���a�擾
			f32 GetFocusRadius() const { return focusRadius; }

			// �A�X�y�N�g��擾
			f32 GetAspect() const { return aspect; }

			// Y����p�擾
			f32 GetFovY() const { return fovY; }

			// �j�A�N���b�v�l�擾
			f32 GetNearZ() const { return nearZ; }

			// �t�@�[�N���b�v�l�擾
			f32 GetFarZ() const { return farZ; }

			f32 GetRotateY() const { return rotateY; }

			void SetCameraShake(Phoenix::Math::Vector3 cameraShake) { shake = cameraShake; }

			// ���_�ʒu�ݒ�
			void SetEye(Math::Vector3 eye) { this->eye = eye; }

			// �����_�ʒu�ݒ�
			void SetFocus(Math::Vector3 focus) { this->focus = focus; }

			void SetRotateX(f32 rotate) { rotateX = rotate; }
			void SetRotateY(f32 rotate) { rotateY = rotate; }

			// ���s���e������
			bool IsOrtho() const { return projection.m[2][3] == 0.0f; }

			// Y����p�ݒ�
			void SetFovY(f32 fov_y);

			// �A�X�y�N�g��ݒ�
			void SetAspect(f32 aspect);

			// �j�A�N���b�v�l�ݒ�
			void SetNearZ(f32 near_z);

			// �t�@�[�N���b�v�l�ݒ�
			void SetFarZ(f32 far_z);

			// �������e�s���ݒ�
			void SetPerspective(f32 fovY, f32 aspect, f32 nearZ, f32 farZ);

			// ���s���e�s���ݒ�
			void SetOrthographic(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ);

			// �r���[�s���ݒ�
			void SetLookAt(const Math::Vector3& eye, const Math::Vector3& focus, const Math::Vector3& up);

			// �r���[�s���ݒ�
			void SetView(const Math::Matrix& view);

			// �v���W�F�N�V�����s���ݒ�
			void SetProjection(const Math::Matrix& projection);

			// �X�N���[���p�r���[�v���W�F�N�V�����s��
			void SetScreen(f32 width, f32 height);

			// compute world dimensions at the given world position.
			void ComputeWorldDimensions(const Math::Vector3& worldPosition, f32& width, f32& height) const;

			// compute unit per pixel at the given world position.
			// posW: world positoin.
			// vh:  view port height in pixels.
			f32 ComputeUnitPerPixel(const Math::Vector3& worldPosition, f32 vh) const;

			// �r���{�[�h�s����v�Z
			void ComputeBillboard(const Math::Vector3& worldPosition, Math::Matrix& billboard) const;

			// ���͈̔͂����E�ɓ���悤�ɃY�[��
			void ZoomOnSphere(const Math::Vector3& center, f32 radius);

			// �t���[�J����
			void FreeCamera();

			void SurveyCamera(f32 addRotateX, f32 addRotateY, f32 distance, Math::Vector3 target);

			void ControllerCamera(const Math::Vector3& center, const Math::Vector3& adjust, const Phoenix::f32 lerpTime = 1.0f);
			void ControllerCamera02(bool onControl, const Math::Vector3& center, const Math::Vector3& adjust, const Phoenix::f32 len, const Phoenix::f32 elapsedTime, const Phoenix::f32 lerpTime = 1.0f, bool adjustRotate = false, const Phoenix::Math::Vector3 targetFrontVec = Phoenix::Math::Vector3());

			void LockOnCamera(const Math::Vector3& center, const Math::Vector3& target, const Math::Vector3& centerAdjust, const Math::Vector3& targetAdjust, bool isLerp = true);

			void SetTargetPos(const Math::Vector3& target, const Math::Vector3& targetAdjust);

			void SphereLinearLockOnCamera(const Math::Vector3& center, const Math::Vector3& start, const Math::Vector3& end, const Math::Vector3& centerAdjust, f32 sphereLinearSpeed, f32 distanceToFouceFromCamera);

			void InitEventCamera(const Math::Vector3& focus, const Math::Vector3& front, Phoenix::f32 len);
			void EventCamera(const Math::Vector3& focus, const Math::Vector3& front, Phoenix::f32 len);

		private:
			// �r���[�x�N�g�����擾
			void GetViewVectors(Math::Vector3& front, Math::Vector3& up);

			// �����̂�ݒ�
			//void SetFructum(const Math::Matrix& view, const Math::Matrix& projection);
		};
	} // namespace Graphics
} // namespace Phoenix