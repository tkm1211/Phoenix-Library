#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"
#include "Phoenix/Graphics/Device.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �o�b�t�@�[�ݒ�L�q
		//****************************************************************************
		typedef
		enum class PhoenixUsage
		{
			Default = 0,
			Immutable = 1,
			Dynamic = 2,
			Staging = 3
		} 	PhoenixUsage;

		typedef
		enum class PhoenixBindFlag
		{
			VertexBuffer = 0x1L,
			IndexBuffer = 0x2L,
			ConstantBuffer = 0x4L,
			ShadowResource = 0x8L,
			StreamOutput = 0x10L,
			RenderTarget = 0x20L,
			DepthStencil = 0x40L,
			UnorderedAccess = 0x80L,
			Decoder = 0x200L,
			VideoEncoder = 0x400L
		} 	PhoenixBindFlag;

		typedef
		enum class PhoenixCPUAccessFlag
		{
			CPUAccessWrite = 0x10000L,
			CPUAccessRead = 0x20000L
		} 	PhoenixCPUAccessFlag;

		typedef
		enum class PhoenixMap
		{
			Read = 1,
			Write = 2,
			ReadWrite = 3,
			WriteDiscard = 4,
			WriteNoOverWrite = 5
		} 	PhoenixMap;

		typedef
		struct PhoenixBufferDesc
		{
			u32 byteWidth;
			PhoenixUsage usage;
			u32 bindFlags;
			u32 cpuAccessFlags;
			u32 miscFlags;
			u32 structureByteStride;
		} 	PhoenixBufferDesc;

		typedef
		struct PhoenixBox
		{
			u32 left;
			u32 top;
			u32 front;
			u32 right;
			u32 bottom;
			u32 back;
		} 	PhoenixBox;

		typedef
		struct PhoenixSubresourceData
		{
			const void* sysMem;
			UINT sysMemPitch;
			UINT sysMemSlicePitch;
		} PhoenixSubresourceData;

		typedef
		struct PhoenixMappedSubresource
		{
			void* data;
			u32 rowPitch;
			u32 depthPitch;
		} PhoenixMappedSubresource;

		//****************************************************************************
		// �o�b�t�@�[����C���^�[�t�F�[�X
		//****************************************************************************
		class IBuffer : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IBuffer> Create();

			// ������
			virtual bool Initialize(IDevice* device, const PhoenixBufferDesc& desc) = 0;
			virtual bool Initialize(IDevice* device, const PhoenixBufferDesc& desc, const PhoenixSubresourceData& data) = 0;

			// �I����
			virtual void Finalize() = 0;
		};
	} // namespace Graphics
} // namespace Phoenix