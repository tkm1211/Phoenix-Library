#pragma once

#include "Phoenix/Graphics/Texture.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// STB�e�N�X�`�����\�[�X�t�@�N�g���N���X
		//****************************************************************************
		class StbTextureResourceFactory : public IStbTextureResourceFactory
		{
		private:
			Graphics::IDevice* device = nullptr;

		public:
			StbTextureResourceFactory(Graphics::IDevice* device) : device(device) {}

			// ���\�[�X�쐬
			std::unique_ptr<OS::Resource> CreateResource(u32 type) override;

			// ���\�[�X�ǂݍ���
			bool LoadResource(OS::Resource* resource, OS::IFileStream* stream, const char* filename)  override;
		};
	} // namespace Graphics
} // namespace Phoenix