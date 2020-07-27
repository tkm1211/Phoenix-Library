#include "pch.h"
#include "RasterizerDX11.h"
#include "Device/Win/DirectX11/DeviceDX11.h"
#include "Phoenix/FND/Assert.h"
#include "Phoenix/FND/Logger.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FND/STD.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�Ń��X�^���C�U�X�e�[�g����I�u�W�F�N�g
		//****************************************************************************
		// ����
		std::unique_ptr<IRasterizer> IRasterizer::Create()
		{
			return std::make_unique<RasterizerDX11>();
		}

		// ������
		bool RasterizerDX11::Initialize(IDevice* device, RasterizerState state, bool enableCull, bool enableDepth, bool enableMultisample, bool enableScissor)
		{
			HRESULT hr;

			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			D3D11_RASTERIZER_DESC desc = {};
			FND::MemSet(&desc, 0, sizeof(desc));
			RenderStateUtillityDX11::MakeD3DRasterizerDesc(state, desc, enableCull, enableDepth, enableMultisample, enableScissor);

			hr = d3dDevice->CreateRasterizerState(&desc, &rasterizerState);
			if (FAILED(hr))
			{
				PHOENIX_LOG_GRP_ERROR("ID3D11Device::CreateRasterizerState() : Failed!!\n");
				return false;
			}

			return true;
		}

		// �I����
		void RasterizerDX11::Finalize()
		{
			FND::SafeRelease(rasterizerState);
		}
	} // namespace Graphics
} // namespace Phoenix