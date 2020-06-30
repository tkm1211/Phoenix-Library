#pragma once

#include "Phoenix/OS/Resource.h"
#include "Phoenix/OS/Display.h"
#include "Phoenix/Graphics/Context.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/Graphics/Renderer.h"
#include "Phoenix/Math/Matrix.h"
#include "Phoenix/Math/Vector.h"
#include "Phoenix/FrameWork/Context/GameContext.h"
#include "Phoenix/FrameWork/CameraRegistry.h"
#include "Phoenix/FrameWork/CollisionManager.h"
#include "Phoenix/FrameWork/frameTime.h"
#include "Phoenix/FrameWork/GameObjectArchive.h"
#include "Phoenix/FrameWork/Input.h"
#include "Phoenix/FrameWork/Object.h"
#include "Phoenix/FrameWork/RenderState.h"
#include "Phoenix/FrameWork/RenderSurface.h"
#include "Phoenix/FrameWork/ShaderLibrary.h"
#include "Phoenix/FrameWork/ShapeLibrary.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// �O���錾
		class GameObject;

		//****************************************************************************
		// �q�b�g�L�^
		//****************************************************************************
		struct HitRecord
		{
			IObject* object;
			GameObject* owner;
			u32 index;
			f32 distance;
			Math::Vector3 hitPt;
			Math::Vector3 normal;
			Math::Vector3 nearestVertex;
			bool hasNormal;
			bool hasNearestVertex;
		};

		//****************************************************************************
		// �Q�[������C���^�[�t�F�[�X
		//****************************************************************************
		class IGame
		{
		public:
			// �Q�[���R���e�L�X�g�擾
			virtual std::shared_ptr<IGameContext> GetGameContext() = 0;

			// �f�B�X�v���C�擾
			virtual OS::IDisplay* GetDisplay() = 0;

			// ���\�[�X�}�l�[�W���[�擾
			virtual OS::IResourceManager* GetResourceManager() = 0;

			// �O���t�B�b�N�X�f�o�C�X�擾
			virtual Graphics::IDevice* GetGraphicsDevice() = 0;

			// �O���t�B�b�N�X�R���e�L�X�g�擾
			virtual Graphics::IContext* GetGraphicsContext() = 0;

			// ���C�������_���擾
			virtual Graphics::ILineRenderer* GetLineRenderer() = 0;

			// �`��X�e�[�g�擾
			virtual RenderState* GetRenderState() = 0;

			// �����_�[�T�[�t�F�C�X�擾
			virtual RenderSurface* GetRenderSurface() = 0;

			// �J�������W�X�g���擾
			virtual CameraRegistry* GetCameraRegistry() = 0;

			// �R���W�����}�l�[�W���[�擾
			virtual CollisionManager* GetCollisionManager() = 0;

			// �C���v�b�g�擾
			virtual Input* GetInput() = 0;

			// �V�F�[�_�[���C�u�����擾
			virtual ShaderLibrary* GetShaderLibrary() = 0;

			// �V�F�C�v���C�u�����擾
			virtual ShapeLibrary* GetShapeLibrary() = 0;

			// �t���[���^�C���擾
			virtual const FrameTime& GetFrameTime() = 0;

			// �X�V�^�C�v�擾
			virtual UpdateType GetUpdateType() = 0;

			// �V���A���C�Y
			virtual void Serialize(std::shared_ptr<GameObject> gameObject, std::ostream& stream, GameObjectArchive archive) = 0;

			// �f�V���A���C�Y
			virtual std::shared_ptr<GameObject> Deserialize(std::istream& stream, GameObjectArchive archive) = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix