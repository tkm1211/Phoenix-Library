#pragma once

#include <memory>
#include <string>
#include "Phoenix/OS/Main.h"
#include "Phoenix/OS/Display.h"
#include "Phoenix/Graphics/GraphicsDevice.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// メインループ
		//****************************************************************************
		class Main : public OS::IMain
		{
		protected:
			std::unique_ptr<OS::IDisplay> display;
			std::shared_ptr<Graphics::IGraphicsDevice> graphicsDevice;
			float elapsedTime = 0.0f;

		public:
			Main() {}
			~Main() {}

		public:
			// 初期化
			bool Initialize(uintPtr instance) override;

			// 終了化
			void Finalize() override;

			// 終了化
			void Run() override;

		protected:
			// 更新
			virtual void Update(Phoenix::f32 elapsedTime) = 0;

			// 描画
			virtual void Render(Phoenix::f32 elapsedTime) = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix