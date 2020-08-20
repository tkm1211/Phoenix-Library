#include "pch.h"
#include "Phoenix/FrameWork/ComputeShader/TestComputeShader.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FND/STD.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// ê∂ê¨
		std::unique_ptr<IComputeShader> TestComputeShader::Create()
		{
			return std::make_unique<TestComputeShader>();
		}

		// èâä˙âª
		bool TestComputeShader::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();

			shader = Graphics::IComputeShader::Create();
			shader->Load(device, "TestComputeShader.cso");

			auto CreateStructuredBuffer = [](Graphics::IDevice* device, u32 elementSize, u32 count, void* initData, Graphics::IBuffer* buffer)
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
					data.sysMem = initData; // TODO : nullptr

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

			auto CreateBufferSRV = [](Graphics::IDevice* device, Graphics::IBuffer* buffer, Graphics::ITexture* shaderResouceView)
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

			auto CreateBufferUAV = [](Graphics::IDevice* device, Graphics::IBuffer* buffer, Graphics::ITexture* shaderResouceView)
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

			for (int i = 0; i < particleMaxSize; i++)
			{
				particle[i].pos = Math::Vector3(0.0f, 0.0f, 0.0f);
				resultParticle[i].pos = Math::Vector3(0.0f, 0.0f, 0.0f);
			}

			particleBuffer01 = Phoenix::Graphics::IBuffer::Create();
			if (!CreateStructuredBuffer(device, sizeof(ParticleParameter), particleMaxSize, &particle[0], particleBuffer01.get()))
			{
				return false;
			}

			particleBufferResult = Phoenix::Graphics::IBuffer::Create();
			if (!CreateStructuredBuffer(device, sizeof(ParticleParameter), particleMaxSize, nullptr, particleBufferResult.get()))
			{
				return false;
			}

			particleBufferSRV01 = Phoenix::Graphics::ITexture::Create();
			if (!CreateBufferSRV(device, particleBuffer01.get(), particleBufferSRV01.get()))
			{
				return false;
			}

			particleBufferUAV = Phoenix::Graphics::ITexture::Create();
			if (!CreateBufferUAV(device, particleBufferResult.get(), particleBufferUAV.get()))
			{
				return false;
			}

			return true;
		}

		// èIóπâª
		void TestComputeShader::Finalize()
		{
			shader.reset();
		}

		// é¿çs
		void TestComputeShader::Run(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();
			Graphics::IContext* context = graphicsDevice->GetContext();

			shader->Activate(device);
			{
				Graphics::ITexture* texture[] = { particleBufferSRV01.get() };
				context->SetShaderResources(Graphics::ShaderType::Compute, 0, 1, texture);

				Graphics::ITexture* uavTexture[] = { particleBufferUAV.get() };
				context->SetUnorderedAccess(0, 1, uavTexture, nullptr);

				shader->Dispatch(device, particleMaxSize, 1, 1);

				Graphics::ITexture* nullUAVTexture[] = { nullptr };
				context->SetUnorderedAccess(0, 1, nullUAVTexture, nullptr);

				Graphics::ITexture* nullTexture[] = { nullptr };
				context->SetShaderResources(Graphics::ShaderType::Compute, 0, 1, nullTexture);
			}
			shader->Deactivate(device);

			auto CreateAndCopyToBuffer = [](Graphics::IDevice* device, Graphics::IContext* context, Graphics::IBuffer* buffer, Graphics::IBuffer* copyBuffer)
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

			std::unique_ptr<Graphics::IBuffer> copyBuffer = Graphics::IBuffer::Create();
			CreateAndCopyToBuffer(device, context, particleBufferResult.get(), copyBuffer.get());

			Graphics::PhoenixMap map = Graphics::PhoenixMap::Read;
			Graphics::PhoenixMappedSubresource mapedBuffer;
			ParticleParameter* result = nullptr;
			{
				context->Map(copyBuffer.get(), 0, map, 0, &mapedBuffer);
				result = static_cast<ParticleParameter*>(mapedBuffer.data);
				context->Unmap(copyBuffer.get(), 0);
			}

			for (int i = 0; i < particleMaxSize; i++)
			{
				resultParticle[i].pos = result[i].pos;
			}
		}
	}
}