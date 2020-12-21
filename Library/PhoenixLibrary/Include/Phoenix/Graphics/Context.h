#pragma once

#include <memory>

#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/Graphics/Surface.h"
#include "Phoenix/Graphics/Viewport.h"
//#include "Phoenix/Graphics/Shader.h"
#include "Phoenix/Graphics/Buffer.h"
#include "Phoenix/Graphics/RenderState.h"
#include "Phoenix/Math/PhoenixMath.h"


namespace Phoenix
{
	namespace Graphics
	{
		// TODO : �ړ�
		typedef enum class ShaderType
		{
			Vertex, // ���_�V�F�[�_�[
			Pixel,  // �s�N�Z���V�F�[�_�[
			Compute,  // �R���s���[�g�V�F�[�_�[

			TypeNum
		} ShaderType;

		//****************************************************************************
		// �R���e�L�X�g����C���^�[�t�F�[�X
		//****************************************************************************
		class IContext : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IContext> Create();

			// ������
			virtual bool Initialize(IDevice* device) = 0;

			// �I����
			virtual void Finalize() = 0;

			// �`��
			virtual void Draw(u32 vertexCount, u32 startVertexLocation) = 0;
			virtual void DrawInstancedIndirect(IBuffer* buffer, u32 offset) = 0;

			// �`��J�n
			virtual void Begin() = 0;

			// �`��I��
			virtual void End() = 0;

			// �`�擯���҂�
			virtual void WaitSync() = 0;

			// �����_�[�^�[�Q�b�g�r���[�N���A
			virtual void ClearRenderTargetView(IRenderTargetSurface* renderTargetSurface, const f32* color) = 0;
			
			// �[�x�X�e���V���r���[�N���A
			virtual void ClearDepthStencilView(IDepthStencilSurface* depthStencilSurface, f32 depth, u8 stencil) = 0;
			
			// �����_�[�^�[�Q�b�g�ݒ�
			virtual void SetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface) = 0;
			
			// �����_�[�^�[�Q�b�g�擾
			virtual void GetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface) = 0;
			
			// �r���[�|�[�g�ݒ�
			virtual void SetViewports(u32 count, const Viewport viewports[]) = 0;
			
			// �r���[�|�[�g�擾
			virtual void GetViewports(u32 count, Viewport* viewports[]) = 0;

			// �V�F�[�_�[���\�[�X�r���[�ݒ�
			virtual void SetShaderResources(ShaderType shadowType, u32 startSlot, u32 numViews, ITexture* texture[]) = 0;

			// �A���I�[�_�[�A�N�Z�X�r���[�ݒ�
			virtual void SetUnorderedAccess(u32 startSlot, u32 numViews, ITexture* texture[], u32* uavInitialCounts) = 0;

			// �u�����h�X�e�[�g�ݒ�
			virtual void SetBlend(IBlend* blend, const f32* blendFactor, u32 samplerMask) = 0;

			// �[�x�X�e���V���X�e�[�g�ݒ�
			virtual void SetDepthStencil(IDepthStencil* depthStencil, u32 stencilRef) = 0;

			// �[�x�X�e���V���X�e�[�g�擾
			virtual void GetDepthStencil(IDepthStencil* depthStencil, u32 stencilRef) = 0;

			// ���X�^���C�U�[�X�e�[�g�ݒ�
			virtual void SetRasterizer(IRasterizer* rasterizer) = 0;

			// ���X�^���C�U�[�X�e�[�g�擾
			virtual void GetRasterizer(IRasterizer* rasterizer) = 0;

			// �T���v���[�X�e�[�g�ݒ�
			virtual void SetSamplers(ShaderType shadowType, u32 startSlot, u32 numViews, ISampler* sampler[]) = 0;

			// �T���v���[�X�e�[�g�擾
			virtual void GetSamplers(ShaderType shadowType, u32 startSlot, u32 numViews, ISampler* sampler[]) = 0;

			// �o�b�t�@�X�V
			virtual void UpdateSubresource(IBuffer* buffer, u32 dstSubresource, const PhoenixBox* dstBox, const void* drcData, u32 srcRowPitch, u32 srcDepthPitch) = 0;
		
			// �o�b�t�@�X�V�J�n
			virtual void Map(IBuffer* buffer, u32 subresource, PhoenixMap mapType, u32 mapFlags, PhoenixMappedSubresource* mappedSubresource) = 0;

			// �o�b�t�@�X�V�I��
			virtual void Unmap(IBuffer* buffer, u32 subresource) = 0;

			// �o�b�t�@�ݒ�
			virtual void SetConstantBuffers(ShaderType shadowType, u32 startSlot, u32 numViews, IBuffer* buffer[]) = 0;

			// �V�[���萔�o�b�t�@�X�V
			virtual void UpdateConstantBufferScene(const Math::Matrix& viewTransform, const Math::Matrix& projectionTransform) = 0;

			// �O�t���[���̃V�[���萔�o�b�t�@�X�V
			virtual void UpdateConstantBufferPrevScene(const Math::Matrix& viewTransform, const Math::Matrix& projectionTransform) = 0;

			// ���b�V���萔�o�b�t�@�X�V
			virtual void UpdateConstantBufferMesh(const Math::Matrix& worldTransform) = 0;

			// �{�[���萔�o�b�t�@�X�V
			virtual void UpdateConstantBufferBone(const Math::Matrix boneTransforms[], s32 boneTransformCount) = 0;

			// �萔�o�b�t�@(�V�[����)�擾
			virtual IBuffer* GetConstantBufferScene() = 0;

			// �萔�o�b�t�@(���b�V����)�擾
			virtual IBuffer* GetConstantBufferMesh() = 0;

			// �萔�o�b�t�@(�{�[���ϊ��s��)�擾
			virtual IBuffer* GetConstantBufferBone() = 0;

			// �u�����h�X�e�[�g�擾
			virtual IBlend* GetBlendState(BlendState type) = 0;

			// �[�x�X�e���V���X�e�[�g�擾
			virtual IDepthStencil* GetDepthStencilState(DepthState type) = 0;

			// ���X�^���C�U�X�e�[�g�擾
			virtual IRasterizer* GetRasterizerState(RasterizerState type) = 0;

			// �T���v���X�e�[�g�擾
			virtual ISampler* GetSamplerState(SamplerState type) = 0;

			// �o�b�t�@�R�s�[
			virtual void CopyResource(IBuffer* destinationBuffer, IBuffer* sourceBuffer) = 0;

			// �~�b�v�}�b�v�쐬
			virtual void GenerateMips(ITexture* texture) = 0;
		};
	} // namespace Graphics
} // namespace Phoenix