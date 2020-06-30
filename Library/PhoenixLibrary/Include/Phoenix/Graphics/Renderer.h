#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"
#include "Phoenix/FND/Flag.h"
#include "Phoenix/Graphics/Context.h"
#include "Phoenix/Graphics/Mesh.h"
#include "Phoenix/Graphics/Renderable.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/OS/Display.h"
#include "Phoenix/Math/Color.h"
#include "Phoenix/Math/Vector.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// ���C�������_������C���^�[�t�F�[�X
		//****************************************************************************
		class ILineRenderer : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<ILineRenderer> Create();

			// ������
			virtual bool Initialize(IDevice* device, u32 vertex_count = 1024) = 0;

			// �I����
			virtual void Finalize() = 0;

			// ���_�ǉ�
			virtual void AddVertex(const Math::Vector3& position, const Math::Color& color = Math::Color::White) = 0;

			// �`��
			virtual void Draw(IContext* context, RenderFlag flags, PrimitiveTopology primitive_topology) = 0;
		};

		//============================================================================
		// �V���h�E�}�b�v�����_������C���^�[�t�F�[�X
		//----------------------------------------------------------------------------
		class IShadowMapRenderer : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IShadowMapRenderer> Create();

			// ������
			virtual bool Initialize(IDevice* device) = 0;

			// �I����
			virtual void Finalize() = 0;

			// �J�n
			virtual void Begin(IContext* context, const Math::AABB& rendered_area) = 0;

			// �`��t���O�ݒ�
			virtual void SetRenderFlag(RenderFlag flags) = 0;

			// �`��
			virtual void Draw(const RenderableNode& node) = 0;

			// �I��
			virtual void End() = 0;
		};

	}	// namespace grp
}	// namespace ys