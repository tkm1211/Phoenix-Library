#pragma once

#include <d3d11.h>
#include <vector>
#include "Phoenix/Graphics/Device.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�Ńf�o�C�X����N���X
		//****************************************************************************
		class DeviceDX11 final : public IDevice
		{
		private:
			ID3D11Device*			device = nullptr;
			ID3D11DeviceContext*	immediateContext = nullptr;
			IDXGIFactory*			dxgiFactory = nullptr;

		public:
			DeviceDX11() {}
			~DeviceDX11() override { Finalize(); }

		public:
			// ������
			bool Initialize() override;

			// �I����
			void Finalize() override;

			// D3D�f�o�C�X�擾
			ID3D11Device* GetD3DDevice() const { return device; }

			// D3D�f�o�C�X�R���e�L�X�g�擾
			ID3D11DeviceContext* GetD3DContext() const { return immediateContext; }

			// DXGI�t�@�N�g���擾
			IDXGIFactory* GetDXGIFactory() const { return dxgiFactory; }
		};
	} // namespace Graphics
} // namespace Phoenix