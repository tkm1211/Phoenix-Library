#pragma once

#include "Phoenix/Graphics/Shader.h"
#include "Phoenix/Graphics/GraphicsDevice.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// コンピュートシェーダー基底クラス
		//****************************************************************************
		class IComputeShader
		{
		public:
			IComputeShader() {}
			virtual ~IComputeShader() {}
		
		public:
			// 生成
			// static std::unique_ptr<IShader> Create(); // 各継承クラスで作成してください。
		
			// 初期化
			virtual bool Initialize(Graphics::IGraphicsDevice* graphicsDevice) = 0;
		
			// 終了化
			virtual void Finalize() = 0;

			// 実行
			virtual void Run(Graphics::IGraphicsDevice* graphicsDevice) = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix