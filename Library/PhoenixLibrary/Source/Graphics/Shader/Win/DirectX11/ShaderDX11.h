#pragma once

#include "Phoenix/Graphics/Shader.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版シェーダー操作オブジェクト
		//****************************************************************************
		class ShaderDX11 final : public IShader
		{
		private:
			ID3D11InputLayout* inputLayout = nullptr;
			ID3D11VertexShader* vertexShader = nullptr;
			ID3D11GeometryShader* geometryShader = nullptr;
			ID3D11PixelShader* pixelShader = nullptr;

		private:
			ID3D11InputLayout* cachedInputLayout = nullptr;
			ID3D11VertexShader* cachedVertexShader = nullptr;
			ID3D11GeometryShader* cachedGeometryShader = nullptr;
			ID3D11PixelShader* cachedPixelShader = nullptr;

		public:
			ShaderDX11() {}
			~ShaderDX11() { Finalize(); }

		public:
			// 初期化
			bool Initialize() override { return true; }

			// 終了化
			void Finalize() override;

			// 頂点シェーダー読み込み
			void LoadVS(IDevice* device, const char* csoNameOfVertexShader, PhoenixInputElementDesc* inputElementDesc, u32 numElements) override;

			// ジオメトリシェーダー読み込み
			void LoadGS(IDevice* device, const char* csoNameOfGeometryShader) override;

			// ピクセルシェーダー読み込み
			void LoadPS(IDevice* device, const char* csoNameOfPixelShader) override;

			// シェーダー開始
			void Activate(IDevice* device) override;

			// シェーダー終了
			void Deactivate(IDevice* device) override;
		};
	} // namespace Graphics
} // namespace Phoenix