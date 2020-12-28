#include "pch.h"
#include "Phoenix/FrameWork/ComputeShader/ComputeShader.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FND/STD.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// �R���s���[�g�V�F�[�_�[�p�o�b�t�@�쐬�֐�
		//****************************************************************************
		bool ComputeShaderBufferFactor::CreateStructuredBuffer(Graphics::IDevice* device, Phoenix::Graphics::PhoenixUsage usage, u32 bindFlags, u32 byteWidth, u32 structureByteStride, s32 miscFlags, void* initData, Graphics::IBuffer* buffer)
		{
			Phoenix::Graphics::PhoenixBufferDesc desc = {};
			Phoenix::FND::MemSet(&desc, 0, sizeof(desc));
			desc.usage = usage;
			desc.bindFlags = bindFlags; // static_cast<Phoenix::u32>(Phoenix::Graphics::PhoenixBindFlag::UnorderedAccess) | static_cast<Phoenix::u32>(Phoenix::Graphics::PhoenixBindFlag::ShadowResource);
			desc.byteWidth = byteWidth;
			desc.miscFlags = miscFlags;
			desc.structureByteStride = structureByteStride;

			if (initData)
			{
				Phoenix::Graphics::PhoenixSubresourceData data = {};
				data.sysMem = initData;

				if (!buffer->Initialize(device, desc, data))
				{
					return false;
				}
			}
			else
			{
				if (!buffer->Initialize(device, desc))
				{
					return false;
				}
			}

			return true;
		}

		//****************************************************************************
		// �R���s���[�g�V�F�[�_�[�p�e�N�X�`���쐬�֐�
		//****************************************************************************
		bool ComputeShaderBufferFactor::CreateBufferSRV(Graphics::IDevice* device, Graphics::IBuffer* buffer, Graphics::ITexture* shaderResouceView, Graphics::TextureFormatDx format, u32 byteWidth, u32 structureByteStride)
		{
			Phoenix::Graphics::PhoenixBufferDesc bufferDesc = {};
			Phoenix::FND::MemSet(&bufferDesc, 0, sizeof(bufferDesc));
			{
				buffer->GetDesc(&bufferDesc);
			}

			Phoenix::Graphics::ShaderResouceDesc srvDesc = {};
			Phoenix::FND::MemSet(&srvDesc, 0, sizeof(srvDesc));
			{
				srvDesc.viewDimension = Graphics::TextureDimensionDx::SRV_DIMENSION_BUFFEREX;
				srvDesc.bufferEx.firstElement = 0;

				if (bufferDesc.miscFlags & Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferAllowsRAWViews)
				{
					srvDesc.format = Graphics::TextureFormatDx::R32_TYPELESS;
					srvDesc.bufferEx.flags = Graphics::BufferExSRVFlag::BufferExSRVFlagRAW;
					srvDesc.bufferEx.numElements = bufferDesc.byteWidth / 4;
				}
				else if (bufferDesc.miscFlags & Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferStructured)
				{
					srvDesc.format = Graphics::TextureFormatDx::UNKNOWN;
					srvDesc.bufferEx.numElements = bufferDesc.byteWidth / bufferDesc.structureByteStride;
				}
				else
				{
					srvDesc.format = format;
					srvDesc.bufferEx.numElements = byteWidth / structureByteStride;
				}
			}
			if (!shaderResouceView->Initialize(device, srvDesc, buffer))
			{
				return false;
			}

			return true;
		};

		bool ComputeShaderBufferFactor::CreateBufferUAV(Graphics::IDevice* device, Graphics::IBuffer* buffer, Graphics::ITexture* shaderResouceView, Graphics::TextureFormatDx format, u32 byteWidth, u32 structureByteStride)
		{
			Phoenix::Graphics::PhoenixBufferDesc bufferDesc = {};
			Phoenix::FND::MemSet(&bufferDesc, 0, sizeof(bufferDesc));
			{
				buffer->GetDesc(&bufferDesc);
			}

			Phoenix::Graphics::UnorderedAccessViewDesc uavDesc = {};
			Phoenix::FND::MemSet(&uavDesc, 0, sizeof(uavDesc));
			{
				uavDesc.viewDimension = Graphics::UAVDimension::BUFFER;
				uavDesc.buffer.firstElement = 0;

				if (bufferDesc.miscFlags & Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferAllowsRAWViews)
				{
					uavDesc.format = Graphics::TextureFormatDx::R32_TYPELESS;
					uavDesc.buffer.flags = Graphics::BufferUAVFlag::BufferUAVFlagRAW;
					uavDesc.buffer.numElements = bufferDesc.byteWidth / 4;
				}
				else if (bufferDesc.miscFlags & Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferStructured)
				{
					uavDesc.format = Graphics::TextureFormatDx::UNKNOWN;
					uavDesc.buffer.numElements = bufferDesc.byteWidth / bufferDesc.structureByteStride;
				}
				else
				{
					return false;
				}
			}
			if (!shaderResouceView->Initialize(device, uavDesc, buffer))
			{
				return false;
			}

			return true;
		};

		void ComputeShaderBufferFactor::CreateAndCopyToBuffer(Graphics::IDevice* device, Graphics::IContext* context, Graphics::IBuffer* buffer, Graphics::IBuffer* copyBuffer)
		{
			std::unique_ptr<Graphics::IBuffer> cloneBuffer = Graphics::IBuffer::Create();

			Phoenix::Graphics::PhoenixBufferDesc bufferDesc = {};
			Phoenix::FND::MemSet(&bufferDesc, 0, sizeof(bufferDesc));
			{
				buffer->GetDesc(&bufferDesc);
			}

			bufferDesc.cpuAccessFlags = Graphics::PhoenixCPUAccessFlag::CPUAccessRead;
			bufferDesc.usage = Graphics::PhoenixUsage::Staging;
			bufferDesc.bindFlags = 0;
			bufferDesc.miscFlags = 0;

			if (copyBuffer->Initialize(device, bufferDesc))
			{
				context->CopyResource(copyBuffer, buffer);
			}
		};
	} // namespace FrameWork
} // namespace Phoenix