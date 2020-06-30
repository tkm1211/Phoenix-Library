#pragma once

#include <vector>
#include <map>
#include "Phoenix/Graphics/Renderable.h"
#include "Phoenix/Math/AABB.h"
#include "Phoenix/FrameWork/Object.h"
#include "Phoenix/FrameWork/RenderState.h"
#include "Phoenix/FrameWork/ShaderLibrary.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// �O���錾
		class IGame;
		class GameObject;

		struct RenderableNode : public Graphics::RenderableNode
		{
			IObject* object = nullptr;
			GameObject* owner = nullptr;
			f32 distance = 0.0f;
		};


		// �`��m�[�h���X�g
		using RenderableNodeList = std::vector<RenderableNode>;

		//****************************************************************************
		// �`��m�[�h�R���N�V����
		//****************************************************************************
		class RenderableCollection : public FND::Base
		{
		protected:
			Math::AABB bounds;

		private:
			GlobalRenderFlag globalRenderFlags = 0;

		public:
			// �`��m�[�h�ǉ�
			virtual void Add(const RenderableNode& node, IGame* game, Graphics::RenderFlag renderFlags, ShaderID shaderID) = 0;

			// �R���N�V�������N���A
			virtual void Clear() = 0;

			// �o�E���f�B���O�{�b�N�X�̎擾
			const Math::AABB& GetBounds() const { return bounds; }

			// �o�E���f�B���O�{�b�N�X�̃N���A
			void ClearBounds() { bounds.Reset(); }
		};

		//****************************************************************************
		// �`��m�[�h�R���N�V����(�\�[�g�Ȃ�)
		//****************************************************************************
		class RenderableSet : public RenderableCollection
		{
		private:
			RenderableNodeList renderables;
			bool skipSelected = false;

		public:
			// �`��m�[�h�ǉ�
			void Add(const RenderableNode& node, IGame* game, Graphics::RenderFlag renderFlags, ShaderID shaderID) override;

			// �R���N�V�������N���A
			void Clear() override;

			// �`��m�[�h���X�g���擾
			RenderableNodeList& GetNodeList() { return renderables; }

			// �I������Ă���m�[�h�͒ǉ�����Ȃ��悤�ɐݒ�
			void SetSkipSelected(bool skipSelected) { this->skipSelected = skipSelected; }
		};

		//****************************************************************************
		// �`��m�[�h�R���N�V����(�\�[�g����)
		//****************************************************************************
		class RenderableSorter : public RenderableCollection
		{
		public:
			// �`��ݒ薈�ɂ܂Ƃ߂��`��m�[�h�f�[�^
			struct Bucket
			{
				ShaderID shaderID;					// �V�F�[�_�[ID
				Graphics::RenderFlag renderFlags;	// �`��t���O
				RenderableNodeList renderables;		// �`��m�[�h���X�g
			};

		private:
			using BucketMap = std::map<u32, Bucket>;
			using BucketKeys = std::vector<u32>;

			BucketMap buckets;
			BucketKeys bucketKeys;

		public:
			// �`��m�[�h�ǉ�
			void Add(const RenderableNode& node, IGame* game, Graphics::RenderFlag renderFlags, ShaderID shaderID) override;

			// �R���N�V�������N���A
			void Clear() override;

			// �o�P�b�g�����擾
			s32 GetBucketCount() const;

			// �o�P�b�g���擾
			Bucket& GetBucket(s32 index);

		private:
			Bucket& GetOrMakeBucket(Graphics::RenderFlag renderFlags, ShaderID shaderID);
		};
	} // namespace FrameWork
} // namespace Phoenix