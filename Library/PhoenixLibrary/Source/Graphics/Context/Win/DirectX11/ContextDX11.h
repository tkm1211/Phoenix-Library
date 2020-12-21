#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <assert.h>
#include "Phoenix/Graphics/Context.h"
#include "Phoenix/Graphics/Surface.h"
#include "Phoenix/Graphics/Viewport.h"
#include "Phoenix/Graphics/Buffer.h"
#include "Phoenix/Math/PhoenixMath.h"
#include "../../../RenderStateUtillity/Win/DirectX11/RenderStateUtillityDX11.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�ŃR���e�L�X�g����I�u�W�F�N�g
		//****************************************************************************
		class ContextDX11 final : public IContext
		{
		public:
			struct CbScene
			{
				Math::Matrix view;
				Math::Matrix projection;
				Math::Matrix viewProjection;
				Math::Matrix viewInv;
				Math::Matrix projectionInv;
				Math::Matrix viewProjectionInv;
				Math::Matrix prevView;
				Math::Matrix prevProjection;
				Math::Matrix prevViewProjection;
				Math::Matrix prevViewInv;
				Math::Matrix prevProjectionInv;
				Math::Matrix prevViewProjectionInv;
				Math::Vector4 viewport;
			};

			struct CbMesh
			{
				Math::Matrix world;
				Math::Matrix worldInverse;
				Math::Matrix texture;
			};

			struct CbBone
			{
				Math::Matrix bones[512];
			};

			struct CbShadow
			{
				Math::Matrix transform;
				f32 texelSize;
				f32 padding[3];
			};

		private:
			ID3D11DeviceContext* deviceContext = nullptr;

			std::unique_ptr<IBuffer> cbScene = nullptr;
			std::unique_ptr<IBuffer> cbMesh = nullptr;
			std::unique_ptr<IBuffer> cbBone = nullptr;

			std::unique_ptr<IBlend> blendState[static_cast<int>(BlendState::TypeNum)] = { nullptr };
			std::unique_ptr<IDepthStencil> depthStencilState[static_cast<int>(DepthState::TypeNum)] = { nullptr };
			std::unique_ptr<IRasterizer> rasterizerState[static_cast<int>(RasterizerState::TypeNum)] = { nullptr };
			std::unique_ptr<ISampler> samplerState[static_cast<int>(SamplerState::TypeNum)] = { nullptr };

			ID3D11Query* query[2] = { nullptr };

			CbScene cbSceneData;

		public:
			ContextDX11() {}
			~ContextDX11() override { Finalize(); }

		public:
			// ������
			bool Initialize(IDevice* device) override;

			// �I����
			void Finalize() override;

			// �`��
			void Draw(u32 vertexCount, u32 startVertexLocation) override;
			void DrawInstancedIndirect(IBuffer* buffer, u32 offset) override;

			// �`��J�n
			void Begin() override;

			// �`��I��
			void End() override;

			// �`�擯���҂�
			void WaitSync() override;

			// �����_�[�^�[�Q�b�g�r���[�N���A
			void ClearRenderTargetView(IRenderTargetSurface* renderTargetSurface, const f32* color) override;
			
			// �[�x�X�e���V���r���[�N���A
			void ClearDepthStencilView(IDepthStencilSurface* depthStencilSurface, f32 depth, u8 stencil) override;
			
			// �����_�[�^�[�Q�b�g�ݒ�
			void SetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface) override;
			
			// �����_�[�^�[�Q�b�g�擾
			void GetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface) override;
			
			// �r���[�|�[�g�ݒ�
			void SetViewports(u32 count, const Viewport viewports[]) override;
			
			// �r���[�|�[�g�擾
			void GetViewports(u32 count, Viewport* viewports[]) override;

			// �V�F�[�_�[���\�[�X�r���[�ݒ�
			void SetShaderResources(ShaderType shadowType, u32 startSlot, u32 numViews, ITexture* texture[]) override;

			// �A���I�[�_�[�A�N�Z�X�r���[�ݒ�
			void SetUnorderedAccess(u32 startSlot, u32 numViews, ITexture* texture[], u32* uavInitialCounts) override;

			// �u�����h�X�e�[�g�ݒ�
			void SetBlend(IBlend* blend, const f32* blendFactor, u32 samplerMask) override;

			// �[�x�X�e���V���X�e�[�g�ݒ�
			void SetDepthStencil(IDepthStencil* depthStencil, u32 stencilRef) override;

			// �[�x�X�e���V���X�e�[�g�擾
			void GetDepthStencil(IDepthStencil* depthStencil, u32 stencilRef) override;

			// ���X�^���C�U�[�X�e�[�g�ݒ�
			void SetRasterizer(IRasterizer* rasterizer) override;

			// ���X�^���C�U�[�X�e�[�g�擾
			void GetRasterizer(IRasterizer* rasterizer) override;

			// �T���v���[�X�e�[�g�ݒ�
			void SetSamplers(ShaderType shadowType, u32 startSlot, u32 numViews, ISampler* sampler[]) override;

			// �T���v���[�X�e�[�g�擾
			void GetSamplers(ShaderType shadowType, u32 startSlot, u32 numViews, ISampler* sampler[]) override;

			// �o�b�t�@�X�V
			void UpdateSubresource(IBuffer* buffer, u32 dstSubresource, const PhoenixBox* dstBox, const void* drcData, u32 srcRowPitch, u32 srcDepthPitch) override;

			// �o�b�t�@�X�V�J�n
			void Map(IBuffer* buffer, u32 subresource, PhoenixMap mapType, u32 mapFlags, PhoenixMappedSubresource* mappedSubresource) override;

			// �o�b�t�@�X�V�I��
			void Unmap(IBuffer* buffer, u32 subresource) override;

			// �o�b�t�@�ݒ�
			void SetConstantBuffers(ShaderType shadowType, u32 startSlot, u32 numViews, IBuffer* buffer[]) override;

			// �V�[���萔�o�b�t�@�X�V
			void UpdateConstantBufferScene(const Math::Matrix& viewTransform, const Math::Matrix& projectionTransform) override;

			// �O�t���[���̃V�[���萔�o�b�t�@�X�V
			void UpdateConstantBufferPrevScene(const Math::Matrix& viewTransform, const Math::Matrix& projectionTransform) override;

			// ���b�V���萔�o�b�t�@�X�V
			void UpdateConstantBufferMesh(const Math::Matrix& worldTransform) override;

			// �{�[���萔�o�b�t�@�X�V
			void UpdateConstantBufferBone(const Math::Matrix boneTransforms[], s32 boneTransformCount) override;

			// �萔�o�b�t�@(�V�[����)�擾
			IBuffer* GetConstantBufferScene() override { return cbScene.get(); }

			// �萔�o�b�t�@(���b�V����)�擾
			IBuffer* GetConstantBufferMesh() override { return cbMesh.get(); }

			// �萔�o�b�t�@(�{�[���ϊ��s��)�擾
			IBuffer* GetConstantBufferBone() override { return cbBone.get(); }

			// �u�����h�X�e�[�g�擾
			IBlend* GetBlendState(BlendState type) override { return blendState[static_cast<int>(type)].get(); }

			// �[�x�X�e���V���X�e�[�g�擾
			IDepthStencil* GetDepthStencilState(DepthState type) override { return depthStencilState[static_cast<int>(type)].get(); }
			
			// ���X�^���C�U�X�e�[�g�擾
			IRasterizer* GetRasterizerState(RasterizerState type) override { return rasterizerState[static_cast<int>(type)].get(); }

			// �T���v���X�e�[�g�擾
			ISampler* GetSamplerState(SamplerState type) override { return samplerState[static_cast<int>(type)].get(); }
		
			// �o�b�t�@�R�s�[
			void CopyResource(IBuffer* destinationBuffer, IBuffer* sourceBuffer) override;

			// �~�b�v�}�b�v�쐬
			void GenerateMips(ITexture* texture) override;
		};
	} // namespace Graphics
} // namespace Phoenix