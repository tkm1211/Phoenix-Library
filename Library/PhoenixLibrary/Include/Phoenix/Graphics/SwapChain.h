#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/Graphics/Surface.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �X���b�v�`�F�[���ݒ�L�q
		//****************************************************************************
		struct SwapChainDesc
		{
			u32 width;
			u32 heigth;
			void* windowHandle;
		};

		//****************************************************************************
		// �X���b�v�`�F�[������C���^�[�t�F�[�X
		//****************************************************************************
		class ISwapChain : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<ISwapChain> Create();

			// ������
			virtual bool Initialize(IDevice* device, const SwapChainDesc& desc) = 0;

			// �I����
			virtual void Finalize() = 0;

			// �����_�����O���ꂽ�C���[�W��\��
			virtual void Present(int syncInterval) = 0;

			// �����_�[�^�[�Q�b�g�T�[�t�F�C�X����I�u�W�F�N�g�擾
			virtual IRenderTargetSurface* GetRenderTargerSurface() = 0;

			// �[�x�X�e���V���T�[�t�F�C�X����I�u�W�F�N�g�擾
			virtual IDepthStencilSurface* GetDepthStencilSurface() = 0;
		};
	} // namespace Graphics
} // namespace Phoenix