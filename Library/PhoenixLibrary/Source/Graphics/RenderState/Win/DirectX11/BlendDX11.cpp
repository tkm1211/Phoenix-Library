#include "pch.h"
#include "BlendDX11.h"
#include "Device/Win/DirectX11/DeviceDX11.h"
#include "Phoenix/FND/Logger.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FND/STD.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�Ńu�����h�X�e�[�g����I�u�W�F�N�g
		//****************************************************************************
		// ����
		std::unique_ptr<IBlend> IBlend::Create()
		{
			return std::make_unique<BlendDX11>();
		}
		
		// ������
		bool BlendDX11::Initialize(IDevice* device, BlendState state)
		{
			HRESULT hr;

			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			D3D11_BLEND_DESC desc = {};
			FND::MemSet(&desc, 0, sizeof(desc));
			RenderStateUtillityDX11::MakeD3DBlendDesc(state, desc);

			hr = d3dDevice->CreateBlendState(&desc, &blendState);
			if (FAILED(hr))
			{
				PHOENIX_LOG_GRP_ERROR("ID3D11Device::CreateBlendState() : Failed!!\n");
				return false;
			}

			return true;
		}
		
		// �I����
		void BlendDX11::Finalize()
		{
			FND::SafeRelease(blendState);
		}
	} // namespace Graphics
} // namespace Phoenix