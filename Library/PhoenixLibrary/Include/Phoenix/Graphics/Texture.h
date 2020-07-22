#pragma once

#include <memory>
#include <vector>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/OS/Resource.h"
#include "Phoenix/Math/Color.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �e�N�X�`�������^�C�v
		//****************************************************************************
		enum class TextureDimension
		{
			Tex1D,		// 1D�e�N�X�`��
			Tex2D,		// 2D�e�N�X�`��
			Tex3D,		// 3D�e�N�X�`��
			Cube,		// �L���[�u�e�N�X�`��
		};

		//****************************************************************************
		// �e�N�X�`���t�H�[�}�b�g
		//****************************************************************************
		enum class TextureFormat
		{
			R8G8B8A8,		//!< 32bit R8G8B8A8
			B8G8R8A8,		//!< 32bit B8G8R8A8
			B4G4R4A4,		//!< 16bit B4G4R4A4
			B5G5R5A1,		//!< 16bit B5G5R5A1
			B5G6R5,			//!< 16bit B5G6R5
			A8,				//!< 8bit  A8
			R8,				//!< 8bit  R8
			BC1,			//!< 4bit  RGB
			BC2,			//!< 8bit  RGBA
			BC3,			//!< 8bit  RGBA

			UNKOWN			//!< �v���b�g�t�H�[���ŗL
		};

		//****************************************************************************
		// �e�N�X�`���ݒ�L�q
		//****************************************************************************
		struct TextureDesc
		{
			u32 width;						// ���B
			u32 height;						// ����(1D�e�N�X�`���̏ꍇ��1)
			u32 depth;						// �[��(1D,2D�e�N�X�`���̏ꍇ��1)
			u32 arraySize;					// �z��
			u32 mipLevels;					// �~�b�v�}�b�v���x���̍ő吔(0�̏ꍇ�͍ő�ɂȂ�)
			TextureDimension dimension;		// �e�N�X�`�������^�C�v
			TextureFormat format;			// �e�N�X�`���t�H�[�}�b�g
			const void* sysMem;				// �������f�[�^�ւ̃|�C���^
			u32	 sysMemSize;				// �������f�[�^�T�C�Y
		};

		//****************************************************************************
		// �}�e���A�����
		//****************************************************************************
		enum class MaterialType
		{
			Diffuse,
			Ambient,
			Specular,
			NormalMap,
			Bump,
			Emissive,
		};

		//****************************************************************************
		// �e�N�X�`������C���^�[�t�F�[�X
		//****************************************************************************
		class ITexture : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<ITexture> Create();

			// ������
			virtual bool Initialize(IDevice* device, const TextureDesc& desc) = 0;

			// ������
			virtual bool Initialize(IDevice* device, const char* filename, MaterialType materialType, const Math::Color& color) = 0;

			// �I����
			virtual void Finalize() = 0;

			// �n���h���擾
			virtual void* Handle() = 0;
		};

		//****************************************************************************
		// �e�N�X�`�����\�[�X����C���^�[�t�F�[�X
		//****************************************************************************
		class ITextureResource : public OS::Resource
		{
		public:
			// �e�N�X�`���擾
			virtual Graphics::ITexture* GetTexture() = 0;

		};

		//****************************************************************************
		// �e�N�X�`�����\�[�X����I�u�W�F�N�g�����C���^�[�t�F�[�X
		//****************************************************************************
		class ITextureResourceFactory : public OS::IResourceFactory
		{
		public:
			// ����
			static std::unique_ptr<ITextureResourceFactory> Create(Graphics::IDevice* device);
		};

		//****************************************************************************
		// STB�e�N�X�`�����\�[�X����I�u�W�F�N�g�����C���^�[�t�F�[�X
		//****************************************************************************
		class IStbTextureResourceFactory : public OS::IResourceFactory
		{
		public:
			// ����
			static std::unique_ptr<IStbTextureResourceFactory> Create(Graphics::IDevice* device);
		};

		//****************************************************************************
		// DDS�e�N�X�`�����\�[�X����I�u�W�F�N�g�����C���^�[�t�F�[�X
		//****************************************************************************
		class IDdsTextureResourceFactory : public OS::IResourceFactory
		{
		public:
			// ����
			static std::unique_ptr<IDdsTextureResourceFactory> Create(Graphics::IDevice* device);
		};

		//****************************************************************************
		// �e�N�X�`���f�[�^
		//****************************************************************************
		struct TextureData
		{
			u32 width;
			u32 height;
			u32 depth;
			u32 arraySize;
			u32 mipLevels;
			Graphics::TextureFormat format;
			Graphics::TextureDimension dimension;

			std::vector<u8> pixels;
		};
	} // namespace Graphics
} // namespace Phoenix