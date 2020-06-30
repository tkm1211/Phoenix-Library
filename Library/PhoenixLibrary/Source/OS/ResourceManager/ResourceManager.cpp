#include "pch.h"
#include <string>
#include "Phoenix/OS/ResourceManager.h"
#include "Phoenix/OS/Path.h"
#include "Phoenix/FND/Assert.h"
#include "Phoenix/FND/Logger.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// ���\�[�X�}�l�[�W���[
		//****************************************************************************
		// ����
		std::unique_ptr<IResourceManager> IResourceManager::Create()
		{
			return std::make_unique<ResourceManager>();
		}

		// ������
		bool ResourceManager::Initialize(const char* rootDirectory)
		{
			file = IFileStream::Create();
			if (!file->Initialize(rootDirectory))
			{
				return false;
			}

			criticalSection = ICriticalSection::Create();
			if (!criticalSection->Initialize())
			{
				return false;
			}

			event = IEvent::Create();
			if (!event->Initialize())
			{
				return false;
			}

			thread = IThread::Create();
			if (!thread->Initialize())
			{
				return false;
			}

			return true;
		}

		// �I����
		void ResourceManager::Finalize()
		{
			GarbageCollect();

			if (criticalSection.get() != nullptr)
			{
				criticalSection->Enter();
			}

			if (thread.get() != nullptr)
			{
				thread->Join();
			}

			if (event.get() != nullptr)
			{
				event->Set();
			}

			thread.reset();
			event.reset();
			file.reset();

			for (auto&& load : loaded)
			{
				load.second.reset();
			}
			loaded.clear();
			for (auto&& pend : pending)
			{
				pend.second.reset();
			}
			pending.clear();

			listeners.clear();
			factories.clear();
		}

		// �g���q�ɑΉ����郊�\�[�X�t�@�N�g���o�^
		void ResourceManager::RegisterFactory(const char* ext, std::shared_ptr<IResourceFactory> factory)
		{
			factories[ext] = factory;
		}

		// ���\�[�X�C�x���g���X�i�[�o�^
		void ResourceManager::RegisterListener(IResourceListener* listener)
		{
			listeners.emplace_back(listener);
		}

		// ���\�[�X����I�u�W�F�N�g�񓯊��ǂݍ���
		std::shared_ptr<Resource> ResourceManager::LoadAsync(const char* filename, u32 type)
		{
			CriticalSectionLock lock(criticalSection.get());

			std::shared_ptr<Resource> resource;

			// �L���b�V���ɑ��݂��邩�`�F�b�N
			{
				auto it = loaded.find(filename);
				if (it != loaded.end())
				{
					resource = it->second;
				}
			}
			// �y���f�B���O�ɑ��݂��邩�`�F�b�N
			if (resource == nullptr)
			{
				auto it = pending.find(filename);
				if (it != pending.end())
				{
					resource = it->second;
				}
			}

			// �V�������\�[�X�����쐬���y���f�B���O�ɒǉ�����
			if (resource == nullptr)
			{
				IResourceFactory* factory = GetFactory(filename);
				if (factory == nullptr)
				{
					return nullptr;
				}

				resource = factory->CreateResource(type);
				pending[filename] = resource;

				event->Set();	// �X���b�h���N����
			}

			return resource;
		}

		// ���\�[�X����I�u�W�F�N�g�����ǂݍ���
		std::shared_ptr<Resource> ResourceManager::LoadImmediate(const char* filename, u32 type)
		{
			CriticalSectionLock lock(criticalSection.get());

			std::shared_ptr<Resource> resource = nullptr;

			// �L���b�V���ɑ��݂��邩�`�F�b�N
			{
				auto it = loaded.find(filename);
				if (it != loaded.end())
				{
					resource = it->second;
				}
			}
			// �y���f�B���O�ɑ��݂��邩�`�F�b�N
			if (resource == nullptr)
			{
				auto it = pending.find(filename);
				if (it != pending.end())
				{
					resource = it->second;
				}
			}
			// �V�������\�[�X�����쐬���A�������[�h����
			if (resource == nullptr)
			{
				IResourceFactory* factory = GetFactory(filename);
				if (factory == nullptr)
				{
					return nullptr;
				}

				resource = factory->CreateResource(type);
				bool load = LoadResource(resource.get(), filename);
				if (!load)
				{
					PHOENIX_LOG_OS_ERROR("���\�[�X�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B: %s\n", filename);
					return nullptr;
				}
				else
				{
					loaded[filename] = resource;
				}
			}

			return resource;
		}

		// �ۗ����̂��ׂẴ��\�[�X���ǂݍ��܂��܂ő҂�
		void ResourceManager::WaitOnPending()
		{
			size_t pendingCount = 0;

			do
			{
				if (criticalSection)
				{
					CriticalSectionLock lock(criticalSection.get());
					pendingCount = pending.size();
				}

				if (pendingCount > 0)
				{
					IThread::Sleep(16);
				}
			} while (pendingCount);
		}

		// �폜���郊�\�[�X������ꍇ�A�������
		int ResourceManager::GarbageCollect()
		{
			s32 numCollected = 0;

			WaitOnPending();
			PHOENIX_ASSERT(pending.size() == 0);

			// WaitOnPending()�̑O�Ń��b�N����ƃf�b�h���b�N�ɂȂ�\��������B
			if (criticalSection)
			{
				CriticalSectionLock lock(criticalSection.get());

				bool foundOne = true;
				while (foundOne)
				{
					foundOne = false;
					auto it = loaded.begin();
					while (it != loaded.end())
					{
						std::shared_ptr<Resource> resource = it->second;
						PHOENIX_ASSERT(resource != nullptr);

						if (resource.use_count() <= 1)
						{
							foundOne = true;
							PHOENIX_LOG_OS_VERVOSE("Unloading %s\n", it->first.c_str());
							resource.reset();
							++numCollected;

							auto temp = it;
							++it;
							loaded.erase(temp);
						}
						else
						{
							++it;
						}
					}
				}
				PHOENIX_LOG_OS_VERVOSE("GarbageCollect completed\n");
				PHOENIX_LOG_OS_VERVOSE("Active Resources# %u\n", loaded.size());
			}

			return numCollected;
		}

		// �X���b�h���s����
		void ResourceManager::Execute()
		{
			std::string filename;
			while (!exitRequested)
			{
				event->Wait();
				if (exitRequested)
				{
					break;
				}

				std::shared_ptr<Resource> resource;
				bool pend = true;
				while (pend)
				{
					// �y���f�B���O���̍ŏ��̃t�@�C�������擾
					{
						CriticalSectionLock lock(criticalSection.get());

						auto it = pending.begin();
						if (it == pending.end())
						{
							pend = false;
							continue;
						}

						PHOENIX_ASSERT(it != pending.end());

						// �N���e�B�J���Z�N�V�����O�Ŏg����悤�ɃR�s�[
						filename = it->first.c_str();
						resource = it->second;
					}

					// ���\�[�X�ǂݍ���
					LoadResource(resource.get(), filename.c_str());

					// �L���b�V���ɒǉ����A�y���f�B���O���폜����
					{
						CriticalSectionLock lock(criticalSection.get());
						loaded[filename.c_str()] = resource;
						pending.erase(filename.c_str());
					}

					// ���X�i�[�Ƀ��[�h�������Ƃ�`����
					for (auto&& listener : listeners)
					{
						listener->OnResourceLoaded(resource.get());
					}
				}
			}
		}

		// �X���b�h�����I��
		void ResourceManager::Exit()
		{
			exitRequested = true;
		}

		// �t�@�N�g���擾
		IResourceFactory* ResourceManager::GetFactory(const char* filename)
		{
			const char* ext = Path::GetFileExtension(filename);

			IResourceFactory* factory = nullptr;
			auto it = factories.find(ext);
			if (it != factories.end())
			{
				factory = it->second.get();
			}
			if (factory == nullptr)
			{
				PHOENIX_LOG_OS_ERROR("�w��̃t�@�C���ɑΉ��������\�[�X�t�@�N�g����������܂���ł����B'%s'\n", filename);
			}

			return factory;
		}

		// ���\�[�X�ǂݍ���
		bool ResourceManager::LoadResource(Resource* resource, const char* filename)
		{
			if (!file->Exists(filename))
			{
				PHOENIX_LOG_OS_ERROR("�w��̃��\�[�X�t�@�C�������݂��܂���B'%s'\n", filename);
				return false;
			}

			IResourceFactory* factory = GetFactory(filename);
			if (factory == nullptr)
			{
				return false;
			}

			bool loaded = factory->LoadResource(resource, file.get(), filename);
			if (loaded)
			{
				resource->SetReady();
				PHOENIX_LOG_OS_VERVOSE("Loaded '%s'\n", filename);
			}
			else
			{
				PHOENIX_LOG_OS_ERROR("�w��̃��\�[�X�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B'%s'\n");
			}

			return loaded;
		}
	} // namespace OS
} // namespace Phoenix