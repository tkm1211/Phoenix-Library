#pragma once

#include <memory>
#include <string>
#include "Phoenix/OS/Main.h"
#include "Phoenix/OS/Display.h"
#include "Phoenix/Graphics/GraphicsDevice.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// ���C�����[�v
		//****************************************************************************
		class Main : public OS::IMain
		{
		protected:
			std::unique_ptr<OS::IDisplay> display;
			std::shared_ptr<Graphics::IGraphicsDevice> graphicsDevice;
			float elapsedTime = 0.0f;

		public:
			Main() {}
			~Main() {}

		public:
			// ������
			bool Initialize(uintPtr instance) override;

			// �I����
			void Finalize() override;

			// �I����
			void Run() override;

		protected:
			// �X�V
			virtual void Update(Phoenix::f32 elapsedTime) = 0;

			// �`��
			virtual void Render(Phoenix::f32 elapsedTime) = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix