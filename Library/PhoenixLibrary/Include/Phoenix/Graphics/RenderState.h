#pragma once

#include <memory>
#include "Phoenix/FND/Base.h"
#include "Phoenix/Graphics/Device.h"
#include "../Source/Graphics/RenderStateUtillity/Win/DirectX11/RenderStateUtillityDX11.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// ブレンドステート操作インターフェース
		//****************************************************************************
		class IBlend : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IBlend> Create();

			// 初期化
			virtual bool Initialize(IDevice* device, BlendState state) = 0;

			// 終了化
			virtual void Finalize() = 0;
		};

		//****************************************************************************
		// 深度ステンシルステート操作インターフェース
		//****************************************************************************
		class IDepthStencil : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IDepthStencil> Create();

			// 初期化
			virtual bool Initialize(IDevice* device, DepthState state) = 0;

			// 終了化
			virtual void Finalize() = 0;
		};

		//****************************************************************************
		// ラスタライザステート操作インターフェース
		//****************************************************************************
		class IRasterizer : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IRasterizer> Create();

			// 初期化
			virtual bool Initialize(IDevice* device, RasterizerState state) = 0;

			// 終了化
			virtual void Finalize() = 0;
		};

		//****************************************************************************
		// サンプラーステート操作インターフェース
		//****************************************************************************
		class ISampler : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<ISampler> Create();

			// 初期化
			virtual bool Initialize(IDevice* device, SamplerState state) = 0;

			// 終了化
			virtual void Finalize() = 0;
		};
	} // namespace Graphics
} // namespace Phoenix