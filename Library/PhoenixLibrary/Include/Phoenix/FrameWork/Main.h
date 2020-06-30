#pragma once

#include <memory>
#include <string>
#include "Phoenix/OS/Main.h"
#include "Phoenix/OS/Display.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/FrameWork/Container.h"


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
			std::unique_ptr<Graphics::IGraphicsDevice> graphicsDevice;

			//bool exit = false;
			//uintPtr handle = 0;
			//std::string name;
			//
			//std::unique_ptr<Container> container;
			//IGameEngine* gameEngine = nullptr;

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

			//// 終了要求
			//void Exit() override { exit = true; }

			//// 終了要求確認
			//bool IsExit() override { return exit; }

			//// 名前取得
			//virtual const char* GetName() override { return name.c_str(); }

			//// 名前設定
			//void SetName(const char* name) override { name = name; }

			//// ハンドル
			//uintPtr Handle() override { return handle; }

		protected:
			// コンテナセットアップ
			//virtual void OnSetupContainer(Container* container);

			// ゲームセットアップ
			//virtual void OnSetupGame(IGame* game);

			// 更新
			virtual void Update() = 0;

			// 描画
			virtual void Render() = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix