#include "pch.h"
#include "Phoenix/FrameWork/ComputeShader/BitonicSort.h"
#include "Phoenix/FND/Logger.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FND/STD.h"


namespace Phoenix
{
	namespace FrameWork
	{
		std::unique_ptr<BitonicSort> BitonicSort::Create()
		{
			return std::make_unique<BitonicSort>();
		}

		bool BitonicSort::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();

			shader = Graphics::IComputeShader::Create();
			shader->Load(device, "BitonicSort.cso");

			buffer = Phoenix::Graphics::IBuffer::Create();
			bufferUAV = Phoenix::Graphics::ITexture::Create();

			constantBuffer = Phoenix::Graphics::IBuffer::Create();
			{
				Phoenix::Graphics::PhoenixBufferDesc desc = {};
				Phoenix::FND::MemSet(&desc, 0, sizeof(desc));
				desc.usage = Phoenix::Graphics::PhoenixUsage::Default;
				desc.bindFlags = static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixBindFlag::ConstantBuffer);
				desc.cpuAccessFlags = 0;
				desc.miscFlags = 0;
				desc.byteWidth = sizeof(ConstantBuffer);
				desc.structureByteStride = 0;
				if (!constantBuffer->Initialize(graphicsDevice->GetDevice(), desc))
				{
					return false;
				}
			}

			return true;
		}

		void BitonicSort::Finalize()
		{
			constantBuffer.reset();
			bufferUAV.reset();
			buffer.reset();
			shader.reset();
		}

		void BitonicSort::Run(Graphics::IGraphicsDevice* graphicsDevice, SortData* data, u32 dataMaxSize)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();
			Graphics::IContext* context = graphicsDevice->GetContext();

			// �o�b�t�@�쐬
			if (!Set(device, data, dataMaxSize))
			{
				PHOENIX_LOG_FRAMEWORK_ERROR("�o�C�g�j�b�N�\�[�g�p�o�b�t�@�̍쐬�Ɏ��s���܂���\n");
				return;
			}

			shader->Activate(device);
			{
				Phoenix::Graphics::IBuffer* psCBuffer[] =
				{
					constantBuffer.get()
				};
				context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Compute, 0, Phoenix::FND::ArraySize(psCBuffer), psCBuffer);

				Graphics::ITexture* uavTexture[] = { bufferUAV.get() };
				context->SetUnorderedAccess(0, 1, uavTexture, nullptr);
				{
					u32 n = dataMaxSize;

					int nLog = static_cast<int>(Phoenix::FND::Log(static_cast<f32>(n), 2.0f));
					int inc;

					for (int i = 0; i < nLog; i++)
					{
						inc = 1 << i;
						for (int j = 0; j < i + 1; j++)
						{
							ConstantBuffer cb = {};
							{
								cb.inc = static_cast<f32>(inc);
								cb.dir = static_cast<f32>(2 << i);
								cb.dummy[0] = 0.0f;
								cb.dummy[1] = 0.0f;
							}
							context->UpdateSubresource(constantBuffer.get(), 0, 0, &cb, 0, 0);

							shader->Dispatch(device, n / 2 / threadNumX, 1, 1);

							inc /= 2;
						}
					}
				}
				Graphics::ITexture* nullUAVTexture[] = { nullptr };
				context->SetUnorderedAccess(0, 1, nullUAVTexture, nullptr);
			}
			shader->Deactivate(device);

			std::unique_ptr<Graphics::IBuffer> copyBuffer = Graphics::IBuffer::Create();
			ComputeShaderBufferFactor::CreateAndCopyToBuffer(device, context, buffer.get(), copyBuffer.get());

			Graphics::PhoenixMap map = Graphics::PhoenixMap::Read;
			Graphics::PhoenixMappedSubresource mapedBuffer;
			SortData* result = nullptr;
			{
				context->Map(copyBuffer.get(), 0, map, 0, &mapedBuffer);
				result = static_cast<SortData*>(mapedBuffer.data);
				context->Unmap(copyBuffer.get(), 0);
			}

			for (u32 i = 0; i < dataMaxSize; i++)
			{
				data[i].key = result[i].key;
				data[i].index = result[i].index;
			}
		}

		bool BitonicSort::Set(Graphics::IDevice* device, SortData* data, u32 dataMaxSize)
		{
			if (!ComputeShaderBufferFactor::CreateStructuredBuffer(device, Phoenix::Graphics::PhoenixUsage::Default, static_cast<Phoenix::u32>(Phoenix::Graphics::PhoenixBindFlag::UnorderedAccess) | static_cast<Phoenix::u32>(Phoenix::Graphics::PhoenixBindFlag::ShaderResource), sizeof(SortData) * dataMaxSize, dataMaxSize, static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferStructured), data, buffer.get()))
			{
				return false;
			}

			if (!ComputeShaderBufferFactor::CreateBufferUAV(device, buffer.get(), bufferUAV.get()))
			{
				return false;
			}

			return true;
		}
	}
}