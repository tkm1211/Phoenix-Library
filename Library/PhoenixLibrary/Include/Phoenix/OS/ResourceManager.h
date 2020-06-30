#pragma once

#include <map>
#include <string>
#include <memory>
#include <vector>
#include "Phoenix/OS/CriticalSection.h"
#include "Phoenix/OS/Resource.h"
#include "Phoenix/OS/Stream.h"
#include "Phoenix/OS/Thread.h"
#include "Phoenix/OS/Event.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// ���\�[�X�}�l�[�W���[
		//****************************************************************************
		class ResourceManager final : public IResourceManager, public IThreadWorker
		{
		private:
			using ResourceInfoMap = std::map<std::string, std::shared_ptr<Resource>>;

			std::map<std::string, std::shared_ptr<IResourceFactory>> factories;
			std::vector<IResourceListener*> listeners;

			std::unique_ptr<ICriticalSection> criticalSection;
			std::unique_ptr<IFileStream> file;
			std::unique_ptr<IThread> thread;
			std::unique_ptr<IEvent> event;

			ResourceInfoMap loaded;
			ResourceInfoMap pending;

			bool exitRequested = false;

		public:
			ResourceManager() {}
			~ResourceManager() override {}

		public:
			// ������
			bool Initialize(const char* rootDirectory) override;

			// �I����
			void Finalize() override;

			// �g���q�ɑΉ����郊�\�[�X�t�@�N�g���o�^
			void RegisterFactory(const char* ext, std::shared_ptr<IResourceFactory> factory) override;

			// ���\�[�X�C�x���g���X�i�[�o�^
			void RegisterListener(IResourceListener* listener) override;

			// ���\�[�X����I�u�W�F�N�g�񓯊��ǂݍ���
			std::shared_ptr<Resource> LoadAsync(const char* filename, u32 type = 0) override;

			// ���\�[�X����I�u�W�F�N�g�����ǂݍ���
			std::shared_ptr<Resource> LoadImmediate(const char* filename, u32 type = 0) override;

			// �ۗ����̂��ׂẴ��\�[�X���ǂݍ��܂��܂ő҂�
			void WaitOnPending() override;

			// �폜���郊�\�[�X������ꍇ�A�������
			int GarbageCollect() override;

			// �X���b�h���s����
			void Execute() override;

			// �X���b�h�����I��
			void Exit() override;

		private:
			// �t�@�N�g���擾
			IResourceFactory* GetFactory(const char* filename);

			// ���\�[�X�ǂݍ���
			bool LoadResource(Resource* resource, const char* filename);
		};
	} // namespace OS
} // namespace Phoenix