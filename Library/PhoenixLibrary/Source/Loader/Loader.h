#pragma once

//#include <fbxsdk.h>
#include "fbxsdk.h"
#include "Phoenix/Loader/Loader.h"


namespace Phoenix
{
	namespace Loader
	{
		//****************************************************************************
		// FBX�ǂݍ��ݑ���I�u�W�F�N�g�N���X
		//****************************************************************************
		class Loader final : public ILoader
		{
		private:
			FbxManager* fbxManager = nullptr;
			FbxScene* fbxScene = nullptr;
			std::string filename;

		public:
			Loader() {}
			~Loader() override { Finalize(); }

		public:
			// ������
			bool Initialize(const char* fileName) override;

			// �I����
			void Finalize() override;

			// ���f���f�[�^�ǂݍ���
			bool Load(Graphics::ModelData& data, const char* dirname) override;

			// �A�j���[�V�����f�[�^�ǂݍ���
			bool Load(Graphics::AnimationData& data) override;

		private:
			// �O�p�`��
			void Triangulate();
		};

	} // namespace Loader
} // namespace Phoenix