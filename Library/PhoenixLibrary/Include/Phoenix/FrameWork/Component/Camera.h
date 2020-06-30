#pragma once

#include "Phoenix/FND/Flag.h"
#include "Phoenix/Math/color.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/Graphics/Material.h"
#include "Phoenix/FrameWork/Component.h"
#include "Phoenix/FrameWork/RenderState.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// �J�����N���A�t���O
		//****************************************************************************
		enum class CameraClearFlags
		{
			SkyBox,
			SolidColor,
			Depth,
			Nothing
		};

		//****************************************************************************
		// �J�����^�C�v
		//****************************************************************************
		enum class CameraType
		{
			Main,
			Sub
		};

		//****************************************************************************
		// �r���[�|�[�g��`
		//****************************************************************************
		struct ViewportRect
		{
			f32					x;
			f32					y;
			f32					w;
			f32					h;

			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};

		//****************************************************************************
		// �J����
		//****************************************************************************
		class Camera : public Component
		{
		private:
			using super = Component;

			Graphics::Camera camera;
			Graphics::StandardMaterial material;
			CameraType cameraType = CameraType::Sub;
			CameraClearFlags clearFlags = CameraClearFlags::SolidColor;
			union
			{
				u32 cullingMaskFlags = CullingMaskFlag::Everything;
				CullingMaskFlag cullingMaskFlag;
			};
			f32 fov = 3.14f / 4.0f;
			f32 aspect = 16.0f / 9.0f;
			f32 nearZ = 0.1f;
			f32 farZ = 1000.0f;
			f32 depth = 0.0f;
			ViewportRect viewportRect = { 0.0f, 0.0f, 1.0f, 1.0f };
			Math::Color backgroundColor = Math::Color::Blue;

		public:
			Camera() {}
			~Camera() override {}

		public:
			// ���O�擾
			const char* GetName() override { return "Camera"; }

			// �������Ɉ�x�����Ă΂��
			void Awake(IGame* game) override;

			// �`��m�[�h���W
			void GetRenderables(IGame* game, RenderableCollection& collection, bool force) override;

			// �s��X�V����
			void UpdateTransform(IGame* game) override;

			// �J�n����
			void Start(IGame* game) override;

			// �j�����ɌĂ΂��
			void OnDestroy(IGame* game) override;

			// �V�[��GUI�\��
			void OnSceneGUI(IGame* game) override;

			// �C���X�y�N�^�[GUI�\��
			void OnInspectorGUI(IGame* game) override;

			// �J�����^�C�v�擾
			CameraType GetCameraType() const { return cameraType; }

			// �J�����^�C�v�ݒ�
			void SetCameraType(CameraType type) { cameraType = type; }

			// �N���A�t���O�擾
			CameraClearFlags GetClearFlags() const { return clearFlags; }

			// �J�����O�}�X�N�t���O�擾
			CullingMaskFlag GetCullingMaskFlags() const { return cullingMaskFlags; }

			// �w�i�F�擾
			const Math::Color& GetBackgroundColor() const { return backgroundColor; }

			// �����_�����O�����擾
			f32 GetDepth() const { return depth; }

			// �����_�����O�����ݒ�
			void SetDepth(f32 depth) { this->depth = depth; }

			// �r���[�|�[�g��`
			const ViewportRect& GetViewportRect() const { return viewportRect; }

			// �r���[�s��擾
			const Math::Matrix& GetView() const { return camera.GetView(); }

			// �v���W�F�N�V�����s��擾
			const Math::Matrix& GetProjection() const { return camera.GetProjection(); }

			// ���_�ʒu�擾
			const Math::Vector3& GetEye() const { return camera.GetEye(); }

			// �����_�ʒu�擾
			const Math::Vector3& GetFocus() const { return camera.GetFocus(); }

			// �O�x�N�g���擾
			const Math::Vector3& GetFront() const { return camera.GetFront(); }

			// ��x�N�g���擾
			const Math::Vector3& GetUp() const { return camera.GetUp(); }

			// �E�x�N�g���擾
			const Math::Vector3& GetRight() const { return camera.GetRight(); }

			// ���_���璍���_�̋����擾
			f32 GetDistanceFromLookAt() const { return camera.GetDistanceFromLookAt(); }

			// ����p�擾
			f32 GetFov() const { return camera.GetFovY(); }

			// ����p�ݒ�
			void SetFov(f32 fov) { camera.SetFovY(fov); }

			// �A�X�y�N�g��擾
			f32 GetAspect() const { return camera.GetAspect(); }

			// �A�X�y�N�g��ݒ�
			void SetAspect(f32 aspect) { camera.SetAspect(aspect); }

			// �j�A�N���b�v�l�擾
			f32 GetNearZ() const { return camera.GetNearZ(); }

			// �j�A�N���b�v�l�ݒ�
			void SetNearZ(f32 near_z) { camera.SetNearZ(near_z); }

			// �t�@�[�N���b�v�l�擾
			f32 GetFarZ() const { return camera.GetFarZ(); }

			// �t�@�[�N���b�v�l�ݒ�
			void SetFarZ(f32 far_z) { camera.SetFarZ(far_z); }

		public:
			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};
	} // namespace FrameWork
} // namespace Phoenix