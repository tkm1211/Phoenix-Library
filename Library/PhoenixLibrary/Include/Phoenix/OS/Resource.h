#pragma once

#include <memory>
#include "Phoenix/types.h"
#include "Phoenix/FND/Base.h"
#include "Phoenix/OS/Stream.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// ���\�[�X������N���X�ł��B
		//****************************************************************************
		class Resource : public FND::Base
		{
		private:
			bool ready = false;

		public:
			// ����������Ԃɐݒ�
			void SetReady() { ready = true; }

			// ����������Ԃ�����
			bool IsReady() const { return ready; }
		};

		//****************************************************************************
		// ���\�[�X����C�x���g���L���b�`����C���^�[�t�F�[�X
		//****************************************************************************
		class IResourceListener : public FND::Base
		{
		public:
			// ���\�[�X���ǂݍ��܂ꂽ�Ƃ��ɌĂ΂��
			virtual void OnResourceLoaded(Resource* resource) = 0;

		};

		//****************************************************************************
		// ���\�[�X����I�u�W�F�N�g�����C���^�[�t�F�[�X
		//****************************************************************************
		class IResourceFactory : public FND::Base
		{
		public:
			// ���\�[�X����I�u�W�F�N�g����
			virtual std::unique_ptr<Resource> CreateResource(u32 type) = 0;

			// ���\�[�X�ǂݍ���
			virtual bool LoadResource(Resource* resource, OS::IFileStream* stream, const char* filename) = 0;

		};

		//****************************************************************************
		// ���\�[�X����I�u�W�F�N�g�Ǘ��C���^�[�t�F�[�X
		//****************************************************************************
		class IResourceManager : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IResourceManager> Create();

			// ������
			virtual bool Initialize(const char* rootDirectory) = 0;

			// �I����
			virtual void Finalize() = 0;

			// �g���q�ɑΉ����郊�\�[�X�t�@�N�g���o�^
			virtual void RegisterFactory(const char* ext, std::shared_ptr<IResourceFactory> factory) = 0;

			// ���\�[�X�C�x���g���X�i�[�o�^
			virtual void RegisterListener(IResourceListener* listener) = 0;

			// ���\�[�X����I�u�W�F�N�g�񓯊��ǂݍ���
			virtual std::shared_ptr<Resource> LoadAsync(const char* filename, u32 type = 0) = 0;

			// ���\�[�X����I�u�W�F�N�g�����ǂݍ���
			virtual std::shared_ptr<Resource> LoadImmediate(const char* filename, u32 type = 0) = 0;

			// �ۗ����̂��ׂẴ��\�[�X���ǂݍ��܂��܂ő҂�
			virtual void WaitOnPending() = 0;

			// �폜���郊�\�[�X������ꍇ�A�������
			virtual int GarbageCollect() = 0;

			// �񓯊��ǂݍ���
			template<class T>
			std::shared_ptr<T> LoadAsync(const char* filename, u32 type = 0)
			{
				return std::dynamic_pointer_cast<T>(LoadAsync(filename, type));
			}

			// �����ǂݍ���
			template<class T>
			std::shared_ptr<T> LoadImmediate(const char* filename, u32 type = 0)
			{
				return std::dynamic_pointer_cast<T>(LoadImmediate(filename, type));
			}
		};

	} // namespace OS
} // namespace Phoenix