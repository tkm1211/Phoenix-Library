#include "pch.h"
#include "Phoenix/FND/Logger.h"
#include "Loader.h"
#include "ModelDataLoader.h"
#include "AnimationDataLoader.h"


namespace Phoenix
{
	namespace Loader
	{
		//****************************************************************************
		// FBX�ǂݍ��ݑ���I�u�W�F�N�g
		//****************************************************************************
		// ����
		std::unique_ptr<ILoader> ILoader::Create()
		{
			return std::make_unique<Loader>();
		}

		// ������
		bool Loader::Initialize(const char* fileName)
		{
			fbxManager = FbxManager::Create();

			// FBX�ɑ΂�����o�͂��`����
			FbxIOSettings* fbxIos = FbxIOSettings::Create(fbxManager, IOSROOT);	// ���ʂȗ��R���Ȃ�����IOSROOT���w��
			fbxManager->SetIOSettings(fbxIos);

			// �C���|�[�^�𐶐�
			FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
			if (!fbxImporter->Initialize(fileName, -1, fbxManager->GetIOSettings()))	// -1�Ńt�@�C���t�H�[�}�b�g��������
			{
				PHOENIX_LOG_FBX_ERROR("FbxImporter::Initialize() : Failed!!\n");
				return false;
			}

			// Scene�I�u�W�F�N�g��FBX�t�@�C�����̏��𗬂�����
			fbxScene = FbxScene::Create(fbxManager, "scene");
			fbxImporter->Import(fbxScene);
			fbxImporter->Destroy();	// �V�[���𗬂����񂾂�Importer�͉������OK

			filename = fileName;

			return true;
		}

		// �I����
		void Loader::Finalize()
		{
			// �}�l�[�W�����
			if (fbxManager != nullptr)
			{
				fbxManager->Destroy();		// �֘A���邷�ׂẴI�u�W�F�N�g����������
				fbxManager = nullptr;
			}
		}

		// ���f���f�[�^�ǂݍ���
		bool Loader::Load(Graphics::ModelData& data, const char* dirname)
		{
			Triangulate();

			ModelDataLoader modelDataLoader;
			return modelDataLoader.Load(fbxScene, data, filename.c_str(), dirname);
		}

		// �A�j���[�V�����f�[�^�ǂݍ���
		bool Loader::Load(Graphics::AnimationData& data)
		{
			AnimationDataLoader animationDataLoader;
			return animationDataLoader.Load(fbxScene, data);
		}

		// �O�p�`��
		void Loader::Triangulate()
		{
			// �W�I���g�����O�p�`������B
			FbxGeometryConverter fbx_geometry_converter(fbxManager);
			fbx_geometry_converter.Triangulate(fbxScene, true);
			fbx_geometry_converter.RemoveBadPolygonsFromMeshes(fbxScene);
		}

	}	// namespace fbx
}	// namespace ys