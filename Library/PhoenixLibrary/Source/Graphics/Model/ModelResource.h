#pragma once

#include "Phoenix/Graphics/Model.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// ���f�����\�[�X�N���X
		//****************************************************************************
		class ModelResource : public IModelResource
		{
		private:
			std::vector<std::unique_ptr<Graphics::IMesh>> meshes;
			ModelData data;

		public:
			// ������
			bool Initialize(Graphics::IDevice* device, const char* filename);

			// ���f���f�[�^�擾
			const ModelData& GetModelData() override { return data; }

			// ���b�V���擾
			IMesh* GetMesh(sizeT index) override { return meshes.at(index).get(); }

			// ���b�V���T�C�Y�擾
			sizeT GetMeshSize() override { return meshes.size(); }
		};

		//****************************************************************************
		// ���f�����\�[�X�t�@�N�g���N���X
		//****************************************************************************
		class ModelResourceFactory : public IModelResourceFactory
		{
		private:
			Graphics::IDevice* device = nullptr;

		public:
			ModelResourceFactory(Graphics::IDevice* device) : device(device) {}

			// ���\�[�X�쐬
			std::unique_ptr<OS::Resource> CreateResource(u32 type) override;

			// ���\�[�X�ǂݍ���
			bool LoadResource(OS::Resource* resource, OS::IFileStream* stream, const char* filename)  override;

		};
	} // namespace Graphics
} // namespace Phoenix