#pragma once

#include <fbxsdk.h>
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/Math/PhoenixMath.h"


namespace Phoenix
{
	namespace Loader
	{
		//****************************************************************************
		// ���f���f�[�^���[�_�[�N���X
		//****************************************************************************
		class ModelDataLoader
		{
		private:
			std::vector<FbxSurfaceMaterial*>	fbxSurfaceMaterials;

		public:
			// ���f���f�[�^�ǂݍ���
			bool Load(FbxScene* fbxScene, Graphics::ModelData& data, const char* fbxFilename, const char* dirname);

		private:
			// �m�[�h����
			void ProcessNodes(FbxNode* fbxNode, Graphics::ModelData& data, s32 parentIndex);

			// �m�[�h����
			void ProcessNode(FbxNode* fbxNode, Graphics::ModelData::Node& node, s32 parentIndex);

			// ���b�V������
			void ProcessMeshes(FbxNode* fbxNode, Graphics::ModelData& data);

			// ���b�V������
			void ProcessMesh(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh, Graphics::ModelData& data);

			// ���_���W����
			void ProcessVertexPositions(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh);

			// ���_�@������
			void ProcessVertexNormals(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh);

			// ���_�ڐ�����
			void ProcessVertexTangents(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh);

			// ���_�e�N�X�`�����W����
			void ProcessVertexTexcoords(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh);

			// ���_�J���[����
			void ProcessVertexColors(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh);

			// ���_�X�L������
			void ProcessVertexSkins(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh, Graphics::ModelData& data);

			// �T�u�Z�b�g����
			void ProcessSubsets(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh);

			// �}�e���A������
			void ProcessMaterials(FbxScene* fbxScene, Graphics::ModelData& data, const char* dirname);

			// �}�e���A������
			void ProcessMaterial(FbxSurfaceMaterial* fbxSurfaceMaterial, Graphics::ModelData::Material& material, const char* dirname);

			// �}�e���A���v���p�e�B�擾
			bool GetMaterialProperty(
				const FbxSurfaceMaterial* fbxSurfaceMaterial,
				const char* fbxColorPropertyName,
				const char* fbxFactorPropertyName,
				FbxDouble3& fbxColor,
				FbxFileTexture*& fbxTexture);

			bool GetMaterialProperty(
				const FbxSurfaceMaterial* fbxSurfaceMaterial,
				const char* fbxColorPropertyName,
				FbxDouble3& fbxColor,
				FbxFileTexture*& fbxTexture);

			bool GetMaterialProperty(
				FbxProperty fbxProperty,
				FbxDouble3& fbxColor,
				FbxFileTexture*& fbxTexture);

			// �}�e���A���t�@�N�^�[�v���p�e�B�擾
			bool GetMaterialFactorProperty(
				const FbxSurfaceMaterial* fbxSurfaceMaterial,
				const char* fbxColorPropertyName,
				FbxDouble& fbxFactor);

			bool GetMaterialFactorProperty(
				FbxProperty fbx_property,
				FbxDouble& fbxFactor);

			// ���_�ڐ���@����UV���W����v�Z����
			void ComputeTangents(Graphics::ModelData::Mesh& mesh);

			// �}�e���A���C���f�b�N�X����
			s32 FindMaterialIndex(FbxScene* fbxScene, FbxSurfaceMaterial* fbxSurfaceMaterial);

			// �m�[�h�C���f�b�N�X������
			static s32 FindNodeIndex(const Graphics::ModelData& data, const char* name);
		};

	} // namespace Loader
} // namespace Phoenix