#pragma once

#include "Phoenix/Graphics/Animation.h"
#include "Phoenix/Math/PhoenixMath.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �A�j���[�V�������\�[�X�N���X
		//****************************************************************************
		class AnimationResource : public IAnimationResource
		{
		private:
			AnimationData data;

		public:
			// �A�j���[�V�����f�[�^�擾
			const AnimationData& GetAnimationData() override { return data; }

			// ������
			bool Initialize(const char* filename);
		};

		//****************************************************************************
		// �A�j���[�V�������\�[�X�t�@�N�g���N���X
		//****************************************************************************
		class AnimationResourceFactory : public IAnimationResourceFactory
		{
		public:
			// ���\�[�X�쐬
			std::unique_ptr<OS::Resource> CreateResource(u32 type) override;

			// ���\�[�X�ǂݍ���
			bool LoadResource(OS::Resource* resource, OS::IFileStream* stream, const char* filename)  override;
		};
	} // namespace Graphics
} // namespace Phoenix