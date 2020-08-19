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
		
			// シェーダー開始
			virtual void Begin(Graphics::IGraphicsDevice* graphicsDevice) = 0;
		
			// シェーダー終了
			virtual void End(Graphics::IGraphicsDevice* graphicsDevice) = 0;
		
			// 描画
			virtual void Dispatch(Graphics::IGraphicsDevice* graphicsDevice, u32 x, u32 y, u32 z) = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix