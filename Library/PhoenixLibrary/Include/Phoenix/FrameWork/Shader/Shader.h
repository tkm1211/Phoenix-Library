#pragma once

#include "Phoenix/Graphics/Shader.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Mesh.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/Graphics/Camera.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// シェーダー基底クラス
		//****************************************************************************
		class IShader
		{
		public:
			IShader() {}
			virtual ~IShader() {}

		public:
			// 生成
			// static std::unique_ptr<IShader> Create(); // 各継承クラスで作成してください。

			// 初期化
			virtual bool Initialize(Graphics::IGraphicsDevice* graphicsDevice) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// シェーダー開始
			virtual void Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera) = 0;

			// シェーダー終了
			virtual void End(Graphics::IGraphicsDevice* graphicsDevice) = 0;

			// 描画
			virtual void Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, ModelObject* model) = 0;

			// 頂点バッファ種類の取得
			virtual Graphics::VertexBufferKind* GetVectexBuferKinds() = 0;

			// 頂点バッファ種類のサイズ
			virtual sizeT GetVectexBuferKindsSize() = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix