#pragma once

#include <vector>
#include "Phoenix/FND/NonCopyable.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// 前方宣言
		class Camera;

		//****************************************************************************
		// カメラレジストリ
		//****************************************************************************
		class CameraRegistry final : public FND::NonCopyable
		{
		private:
			std::vector<Camera*> cameras;
			Camera* mainCamera = nullptr;

		public:
			// カメラ登録
			void RegisterCamera(Camera* camera);

			// カメラ登録解除
			void UnregisterCamera(Camera* camera);

			// 登録クリア
			void Clear();

			// ソート
			void Sort();

			// カメラリスト取得
			const std::vector<Camera*>& GetCameras() const { return cameras; }

			// メインカメラ取得
			Camera* GetMainCamera() const { return mainCamera; }
		};
	} // namespace FrameWork
} // namespace Phoenix