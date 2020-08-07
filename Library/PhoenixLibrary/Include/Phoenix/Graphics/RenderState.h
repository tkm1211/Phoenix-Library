#pragma once

#include <memory>
#include "Phoenix/FND/Base.h"
#include "Phoenix/Graphics/Device.h"
#include "../Source/Graphics/RenderStateUtillity/Win/DirectX11/RenderStateUtillityDX11.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �u�����h�X�e�[�g����C���^�[�t�F�[�X
		//****************************************************************************
		class IBlend : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IBlend> Create();

			// ������
			virtual bool Initialize(IDevice* device, BlendState state) = 0;

			// �I����
			virtual void Finalize() = 0;
		};

		//****************************************************************************
		// �[�x�X�e���V���X�e�[�g����C���^�[�t�F�[�X
		//****************************************************************************
		class IDepthStencil : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IDepthStencil> Create();

			// ������
			virtual bool Initialize(IDevice* device, DepthState state) = 0;

			// �I����
			virtual void Finalize() = 0;
		};

		//****************************************************************************
		// ���X�^���C�U�X�e�[�g����C���^�[�t�F�[�X
		//****************************************************************************
		class IRasterizer : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IRasterizer> Create();

			// ������
			virtual bool Initialize(IDevice* device, RasterizerState state, bool enableCull = true, bool enableDepth = true, bool enableMultisample = false, bool enableScissor = false) = 0;

			// �I����
			virtual void Finalize() = 0;
		};

		//****************************************************************************
		// �T���v���[�X�e�[�g����C���^�[�t�F�[�X
		//****************************************************************************
		class ISampler : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<ISampler> Create();

			// ������
			virtual bool Initialize(IDevice* device, SamplerState state, bool enableAlways = true, bool enableComparison = false) = 0;

			// �I����
			virtual void Finalize() = 0;
		};
	} // namespace Graphics
} // namespace Phoenix