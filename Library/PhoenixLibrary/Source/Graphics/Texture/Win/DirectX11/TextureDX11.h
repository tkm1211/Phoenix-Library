#pragma once

#include <d3d11.h>
#include "Phoenix/Graphics/Texture.h"


namespace Phoenix
{
	namespace Graphics
	{
		//============================================================================
		// DirectX11�Ńe�N�X�`������I�u�W�F�N�g
		//----------------------------------------------------------------------------
		class TextureDX11 final : public ITexture
		{
		private:
			union
			{
				ID3D11Resource* resource = nullptr;
				ID3D11Texture1D* texture1d;
				ID3D11Texture2D* texture2d;
				ID3D11Texture3D* texture3d;
			};
			ID3D11ShaderResourceView* shaderResourceView = nullptr;

		public:
			TextureDX11() {}
			~TextureDX11() override { Finalize(); }

			// ������
			bool Initialize(ID3D11Texture2D* texture, ID3D11ShaderResourceView* d3dShaderResourceView);

			// ������
			bool Initialize(IDevice* device, const TextureDesc& desc) override;

			// ������
			bool Initialize(IDevice* device, const char* filename) override;

			// �I����
			void Finalize() override;

			// �n���h���擾
			void* Handle() override { return shaderResourceView; }

			// D3D�e�N�X�`���擾
			ID3D11Texture2D* GetD3DTexture() const { return texture2d; }

			// D3D�V�F�[�_�[���\�[�X�r���[�擾
			ID3D11ShaderResourceView* GetD3DShaderResourceView() const { return shaderResourceView; }

			// D3D�V�F�[�_�[���\�[�X�r���[�ݒ�
			void SetD3DShaderResourceView(ID3D11ShaderResourceView* d3dShaderResourceView) { shaderResourceView = d3dShaderResourceView; }

		private:
			// DXGI�t�H�[�}�b�g�擾
			static DXGI_FORMAT GetDXGIFormat(TextureFormat format);

			// D3D���\�[�X�����擾
			static D3D11_RESOURCE_DIMENSION GetD3DResourceDimension(TextureDimension dimension);

			// 1�s�N�Z��������̃r�b�g���擾
			static UINT BitsPerPixel(DXGI_FORMAT dxgiformat);

			// �T�[�t�F�C�X���擾
			static void GetSurfaceInfo(UINT width, UINT height, DXGI_FORMAT dxgiformat, UINT* outNumBytes, UINT* outRowBytes, UINT* outNumRows);

			// SRGB�t�H�[�}�b�g�֕ϊ�
			static DXGI_FORMAT MakeSRGB(DXGI_FORMAT dxgiFormat);

			// �����f�[�^��ݒ�
			static HRESULT FillInitData(
				UINT width,
				UINT height,
				UINT depth,
				UINT mipCount,
				UINT arraySize,
				DXGI_FORMAT dxgiFormat,
				UINT maxsize,
				UINT bitSize,
				const uint8_t* bitData,
				UINT& twidth,
				UINT& theight,
				UINT& tdepth,
				UINT& skipMip,
				D3D11_SUBRESOURCE_DATA* subresourceDatas);

			// D3D���\�[�X�쐬
			static HRESULT CreateD3DResources(
				ID3D11Device* device,
				D3D11_RESOURCE_DIMENSION resourceDimention,
				UINT width,
				UINT height,
				UINT depth,
				UINT mipCount,
				UINT arraySize,
				DXGI_FORMAT dxgiFormat,
				D3D11_USAGE d3dUsage,
				UINT bindFlags,
				UINT cpuAccessFlags,
				UINT miscFlags,
				bool forceSrgb,
				bool isCubeMap,
				D3D11_SUBRESOURCE_DATA* subresourceDatas,
				ID3D11Resource** resource,
				ID3D11ShaderResourceView** shaderResourceView);
		};
	} // namespace Graphics
} // namespace Phoenix