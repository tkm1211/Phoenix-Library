#pragma once

#include <string>
#include "Phoenix/Graphics/Material.h"


namespace Phoenix
{
	namespace FrameWork
	{
		using ShaderID = s32;

		// �O���錾
		class IGame;

		//****************************************************************************
		// �}�e���A������C���^�[�t�F�[�X
		//****************************************************************************
		class IMaterial
		{
		public:
			// �}�e���A���擾
			virtual Graphics::Material* GetMaterial() = 0;

			// ��������
			virtual bool HasTransparency() = 0;

			// �Z�b�g�A�b�v
			virtual void Setup(IGame* game) = 0;

			// �C���X�y�N�^�[GUI�\��
			virtual void OnInspectorGUI(IGame* game) = 0;
		};

		//****************************************************************************
		// �}�e���A�����
		//****************************************************************************
		struct MaterialInfo
		{
			ShaderID shaderID = 0;
			std::string name;
			std::shared_ptr<IMaterial> material;

			// �C���X�y�N�^�[GUI�\��
			void OnInspectorGUI(IGame* game);

			template<class Archive>
			void serialize(Archive& archive, u32 version);

		private:
			bool open = false;
		};

		//****************************************************************************
		// �x�[�V�b�N�}�e���A��
		//****************************************************************************
		class BasicMaterial : public IMaterial
		{
		private:
			Graphics::BasicMaterial	material;

		public:
			// �}�e���A���擾
			Graphics::Material* GetMaterial() override { return &material; }

			// ��������
			bool HasTransparency() override { return false; }

			// �Z�b�g�A�b�v
			void Setup(IGame* game) override {}

			// �C���X�y�N�^�[GUI�\��
			void OnInspectorGUI(IGame* game) override;

			// �J���[�ݒ�
			void SetColor(const Math::Color& color) { material.color = color; }

			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};

		//****************************************************************************
		// �X�^���_�[�h�}�e���A��
		//****************************************************************************
		class StandardMaterial : public IMaterial
		{
		private:
			Graphics::StandardMaterial material;
			std::string diffusemapFilename;
			std::string normalmapFilename;
			std::string specularmapFilename;
			std::shared_ptr<Graphics::ITextureResource>	diffusemapResource;
			std::shared_ptr<Graphics::ITextureResource>	normalmapResource;
			std::shared_ptr<Graphics::ITextureResource>	specularmapResource;

		public:
			// �}�e���A���擾
			Graphics::Material* GetMaterial() override { return &material; }

			// ��������
			bool HasTransparency() override { return false; }

			// �Z�b�g�A�b�v
			void Setup(IGame* game) override;

			// �C���X�y�N�^�[GUI�\��
			void OnInspectorGUI(IGame* game) override;

			// �G�~�b�V�u�J���[�ݒ�
			void SetEmissiveColor(const Math::Color& color) { material.emissive = color; }

			// �f�B�t���[�Y�J���[�ݒ�
			void SetDiffuseColor(const Math::Color& color) { material.diffuse = color; }

			// �X�y�L�����J���[�ݒ�
			void SetSpecularColor(const Math::Color& color) { material.specular = color; }

			// �f�B�t���[�Y�}�b�v�e�N�X�`���ǂݍ���
			void LoadDiffuseMap(IGame* game, const char* filename);

			// �@���}�b�v�e�N�X�`���ǂݍ���
			void LoadNormalMap(IGame* game, const char* filename);

			// �X�y�L�����}�b�v�e�N�X�`���ǂݍ���
			void LoadSpecularMap(IGame* game, const char* filename);

		public:
			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};

		//****************************************************************************
		// �X�J�C�h�[���}�e���A��
		//****************************************************************************
		class SkyDomeMaterial : public IMaterial
		{
		private:
			Graphics::SkyDomeMaterial material;
			std::string cubemapFilename;
			std::shared_ptr<Graphics::ITextureResource>	textureResource;

		public:
			// �}�e���A���擾
			Graphics::Material* GetMaterial() override { return &material; }

			// ��������
			bool HasTransparency() override { return false; }

			// �Z�b�g�A�b�v
			void Setup(IGame* game) override;

			// �C���X�y�N�^�[GUI�\��
			void OnInspectorGUI(IGame* game) override;

		private:
			// �X�J�C�h�[���e�N�X�`���ǂݍ���
			void LoadCubeMap(IGame* game, const char* filename);

		public:
			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};
	} // namespace FrameWork
} // namespace Phoenix