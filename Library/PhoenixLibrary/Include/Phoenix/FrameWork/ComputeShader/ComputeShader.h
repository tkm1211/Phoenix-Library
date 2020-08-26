#pragma once

#include "Phoenix/Graphics/Shader.h"
#include "Phoenix/Graphics/GraphicsDevice.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// コンピュートシェーダー用バッファ作成関数
		//****************************************************************************
		bool CreateStructuredBuffer(Graphics::IDevice* device, u32 elementSize, u32 count, void* initData, Graphics::IBuffer* buffer);

		//****************************************************************************
		// コンピュートシェーダー用テクスチャ作成関数
		//****************************************************************************
		bool CreateBufferSRV(Graphics::IDevice* device, Graphics::IBuffer* buffer, Graphics::ITexture* shaderResouceView);

		bool CreateBufferUAV(Graphics::IDevice* device, Graphics::IBuffer* buffer, Graphics::ITexture* shaderResouceView);

		//****************************************************************************
		// コンピュートシェーダー用データ抽出関数
		//****************************************************************************
		void CreateAndCopyToBuffer(Graphics::IDevice* device, Graphics::IContext* context, Graphics::IBuffer* buffer, Graphics::IBuffer* copyBuffer);

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