#include "Phoenix/FrameWork/ComputeShader/TestComputeShader.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FND/STD.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// 生成
		std::unique_ptr<IComputeShader> TestComputeShader::Create()
		{
			return std::make_unique<TestComputeShader>();
		}

		// 初期化
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

				Phoenix::Graphics::PhoenixSubresourceData data = {};
				data.sysMem = initData;

				if (!buffer->Initialize(device, desc, data))
				{
					return false;
				}

				return true;
			};

			srv = Phoenix::Graphics::IBuffer::Create();
			if (CreateStructuredBuffer(device, sizeof(ParticleParameter), particleMaxSize, &particle[0], srv.get()))
			{
				return false;
			}

			uav = Phoenix::Graphics::IBuffer::Create();
			if (CreateStructuredBuffer(device, sizeof(ParticleParameter), particleMaxSize, nullptr, uav.get()))
			{
				return false;
			}
		}

		// 終了化
		void TestComputeShader::Finalize()
		{
			shader.reset();
		}

		// シェーダー開始
		void TestComputeShader::Begin(Graphics::IGraphicsDevice* graphicsDevice)
		{
			shader->Activate(graphicsDevice->GetDevice());
		}

		// シェーダー終了
		void TestComputeShader::End(Graphics::IGraphicsDevice* graphicsDevice)
		{
			shader->Deactivate(graphicsDevice->GetDevice());
		}

		// 実行
		void TestComputeShader::Dispatch(Graphics::IGraphicsDevice* graphicsDevice, u32 x, u32 y, u32 z)
		{
			shader->Dispatch(graphicsDevice->GetDevice(), x, y, z);
		}
	}
}