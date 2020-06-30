#pragma once

#include <vector>
#include "Phoenix/FND/NonCopyable.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// �O���錾
		class Camera;

		//****************************************************************************
		// �J�������W�X�g��
		//****************************************************************************
		class CameraRegistry final : public FND::NonCopyable
		{
		private:
			std::vector<Camera*> cameras;
			Camera* mainCamera = nullptr;

		public:
			// �J�����o�^
			void RegisterCamera(Camera* camera);

			// �J�����o�^����
			void UnregisterCamera(Camera* camera);

			// �o�^�N���A
			void Clear();

			// �\�[�g
			void Sort();

			// �J�������X�g�擾
			const std::vector<Camera*>& GetCameras() const { return cameras; }

			// ���C���J�����擾
			Camera* GetMainCamera() const { return mainCamera; }
		};
	} // namespace FrameWork
} // namespace Phoenix