#pragma once

#include <d3d11.h>
#include "Phoenix/Graphics/Buffer.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�Ńo�b�t�@�[����I�u�W�F�N�g
		//****************************************************************************
		class BufferDX11 final : public IBuffer
		{
		private:
			ID3D11Buffer* buffer = nullptr;

		public:
			BufferDX11() {}
			~BufferDX11() { Finalize(); }

		public:
			// ������
			bool Initialize(IDevice* device, const PhoenixBufferDesc& desc) override;
			bool Initialize(IDevice* device, const PhoenixBufferDesc& desc, const PhoenixSubresourceData& data) override;

			// �I����
			void Finalize() override;

			// ���擾
			void GetDesc(PhoenixBufferDesc* desc) override;

			// D3D�o�b�t�@�[�擾
			ID3D11Buffer* GetD3DBuffer() { return buffer; }
		};
	} // namespace Graphics
} // namespace Phoenix