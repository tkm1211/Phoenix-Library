#include "pch.h"
#include "TextureResource.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �e�N�X�`�����\�[�X�N���X
		//****************************************************************************
		// ������
		bool TextureResource::Initialize(Graphics::IDevice* device, const TextureDesc& desc)
		{
			texture = ITexture::Create();
			if (!texture->Initialize(device, desc))
			{
				return false;
			}

			return true;
		}

		//****************************************************************************
		// �e�N�X�`�����\�[�X�N���X
		//****************************************************************************
		// ����
		std::unique_ptr<ITextureResourceFactory> ITextureResourceFactory::Create(Graphics::IDevice* device)
		{
			return std::make_unique<TextureResourceFactory>(device);
		}

		// ���\�[�X�쐬
		std::unique_ptr<OS::Resource> TextureResourceFactory::CreateResource(u32 type)
		{
			return std::make_unique<TextureResource>();
		}

		// ���\�[�X�ǂݍ���
		bool TextureResourceFactory::LoadResource(OS::Resource* resource, OS::IFileStream* stream, const char* filename)
		{
			return false;
		}

	} // namespace Graphics
} // namespace Phoenix