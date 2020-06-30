#include "pch.h"
#include <list>
#include "ShaderDX11.h"
#include "Phoenix/FND/Util.h"
#include "../Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "../Graphics/Context/Win/DirectX11/ContextDX11.h"
#include "../Source/Loader/ResourceManager.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版グラフィックスデバイス操作オブジェクト
		//****************************************************************************
		// 生成
		std::unique_ptr<IShader> IShader::Create()
		{
			return std::make_unique<ShaderDX11>();
		}

		// 終了化
		void ShaderDX11::Finalize()
		{
			FND::SafeRelease(inputLayout);
			FND::SafeRelease(pixelShader);
			FND::SafeRelease(geometryShader);
			FND::SafeRelease(vertexShader);
		}

		// 頂点シェーダー読み込み
		void ShaderDX11::LoadVS(IDevice* device, const char* csoNameOfVertexShader, PhoenixInputElementDesc* inputElementDesc, u32 numElements)
		{
			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			std::vector<D3D11_INPUT_ELEMENT_DESC> descList;
			for (u32 i = 0; i < numElements; i++)
			{
				D3D11_INPUT_ELEMENT_DESC desc = {};
				desc.SemanticName = static_cast<LPCSTR>(inputElementDesc[i].semanticName);
				desc.Format = static_cast<DXGI_FORMAT>(inputElementDesc[i].format);
				desc.InputSlotClass = static_cast<D3D11_INPUT_CLASSIFICATION>(inputElementDesc[i].inputSlotClass);
				desc.SemanticIndex = inputElementDesc[i].semanticIndex;
				desc.InputSlot = inputElementDesc[i].inputSlot;
				desc.AlignedByteOffset = inputElementDesc[i].alignedByteOffset;
				desc.InstanceDataStepRate = inputElementDesc[i].instanceDataStepRate;

				descList.emplace_back(desc);
			}

			if (csoNameOfVertexShader) ResourceManager::CreateVertexShaderAndInputLayout(d3dDevice, csoNameOfVertexShader, &vertexShader, &inputLayout, descList.data(), numElements);
		}

		// ジオメトリシェーダー読み込み
		void ShaderDX11::LoadGS(IDevice* device, const char* csoNameOfGeometryShader)
		{
			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();
			if (csoNameOfGeometryShader) ResourceManager::CreateGeometryShader(d3dDevice, csoNameOfGeometryShader, &geometryShader);
		}

		// ピクセルシェーダー読み込み
		void ShaderDX11::LoadPS(IDevice* device, const char* csoNameOfPixelShader)
		{
			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();
			if (csoNameOfPixelShader) ResourceManager::CreatePixelShader(d3dDevice, csoNameOfPixelShader, &pixelShader);
		}

		// シェーダー開始
		void ShaderDX11::Activate(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();

			d3dDeviceContext->IAGetInputLayout(&cachedInputLayout);
			d3dDeviceContext->VSGetShader(&cachedVertexShader, 0, 0);
			d3dDeviceContext->GSGetShader(&cachedGeometryShader, 0, 0);
			d3dDeviceContext->PSGetShader(&cachedPixelShader, 0, 0);

			if (vertexShader)
			{
				d3dDeviceContext->IASetInputLayout(inputLayout);
				d3dDeviceContext->VSSetShader(vertexShader, 0, 0);
			}
			if (geometryShader)
			{
				d3dDeviceContext->GSSetShader(geometryShader, 0, 0);
			}
			if (pixelShader)
			{
				d3dDeviceContext->PSSetShader(pixelShader, 0, 0);
			}
		}

		// シェーダー終了
		void ShaderDX11::Deactivate(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();

			d3dDeviceContext->IASetInputLayout(cachedInputLayout);
			d3dDeviceContext->VSSetShader(cachedVertexShader, 0, 0);
			d3dDeviceContext->GSSetShader(cachedGeometryShader, 0, 0);
			d3dDeviceContext->PSSetShader(cachedPixelShader, 0, 0);
		}
	} // namespace Graphics
} // namespace Phoenix