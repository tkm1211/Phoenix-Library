#include "pch.h"
#include "Phoenix/FrameWork/ComputeShader/ComputeShader.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FND/STD.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// コンピュートシェーダー用バッファ作成関数
		//****************************************************************************
		bool CreateStructuredBuffer(Graphics::IDevice* device, u32 elementSize, u32 count, void* initData, Graphics::IBuffer* buffer)
		{
			Phoenix::Graphics::PhoenixBufferDesc desc = {};
			Phoenix::FND::MemSet(&desc, 0, sizeof(desc));
			desc.usage = Phoenix::Graphics::PhoenixUsage::Default;
			desc.bindFlags = static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixBindFlag::UnorderedAccess) | static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixBindFlag::ShadowResource);
			desc.byteWidth = elementSize * count;
			desc.miscFlags = static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferStructured);
			desc.structureByteStride = elementSize;

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
		};

		//****************************************************************************
		// コンピュートシェーダー用テクスチャ作成関数
		//****************************************************************************
		bool CreateBufferSRV(Graphics::IDevice* device, Graphics::IBuffer* buffer, Graphics::ITexture* shaderResouceView)
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
					return false;
				}
			}
			if (!shaderResouceView->Initialize(device, srvDesc, buffer))
			{
				return false;
			}

			return true;
		};

		bool CreateBufferUAV(Graphics::IDevice* device, Graphics::IBuffer* buffer, Graphics::ITexture* shaderResouceView)
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

		void CreateAndCopyToBuffer(Graphics::IDevice* device, Graphics::IContext* context, Graphics::IBuffer* buffer, Graphics::IBuffer* copyBuffer)
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