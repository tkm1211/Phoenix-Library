#pragma once

#include "Phoenix/Graphics/Texture.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �e�N�X�`�����\�[�X�N���X
		//****************************************************************************
		class TextureResource : public ITextureResource
		{
		private:
			std::unique_ptr<Graphics::ITexture> texture = nullptr;

		public:
			// ������
			bool Initialize(Graphics::IDevice* device, const TextureDesc& desc);

			// �e�N�X�`���擾
			Graphics::ITexture* GetTexture() override { return texture.get(); }
		};

		//****************************************************************************
		// �e�N�X�`�����\�[�X�N���X
		//****************************************************************************
		class TextureResourceFactory : public ITextureResourceFactory
		{
		private:
			Graphics::IDevice* device = nullptr;

		public:
			TextureResourceFactory(Graphics::IDevice* device) : device(device) {}

			// ���\�[�X�쐬
			std::unique_ptr<OS::Resource> CreateResource(u32 type) override;

			// ���\�[�X�ǂݍ���
			bool LoadResource(OS::Resource* resource, OS::IFileStream* stream, const char* filename)  override;
		};
	} // namespace Graphics
} // namespace Phoenix