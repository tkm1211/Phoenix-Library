#include "pch.h"
#include <algorithm>
#include "Phoenix/FrameWork/Component/Camera.h"
#include "Phoenix/FrameWork/CameraRegistry.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// �J�������W�X�g��
		//****************************************************************************
		// �J�����o�^
		void CameraRegistry::RegisterCamera(Camera* camera)
		{
			cameras.emplace_back(camera);
		}

		// �J�����o�^����
		void CameraRegistry::UnregisterCamera(Camera* camera)
		{
			auto&& it = std::find(cameras.begin(), cameras.end(), camera);
			if (it != cameras.end())
			{
				cameras.erase(it);
			}
		}

		// �o�^�N���A
		void CameraRegistry::Clear()
		{
			cameras.clear();
		}

		// �\�[�g
		void CameraRegistry::Sort()
		{
			std::sort(cameras.begin(), cameras.end(), [](Camera* a, Camera* b) { return a->GetDepth() < b->GetDepth(); });

			for (Camera* camera : cameras)
			{
				if (camera->GetCameraType() == CameraType::Main)
				{
					mainCamera = camera;
					return;
				}
			}
			mainCamera = nullptr;
		}
	} // namespace FrameWork
} // namespace Phoenix