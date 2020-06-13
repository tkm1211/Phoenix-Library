#pragma once

#include <memory>
#include "Phoenix/OS/Main.h"
#include "Phoenix/OS/Display.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/Graphics/Context.h"
#include "Phoenix/Graphics/SwapChain.h"


namespace Phoenix
{
	namespace FrameWork
	{
		class Main : public OS::IMain
		{
		private:
			std::unique_ptr<OS::IDisplay> display;
			std::unique_ptr<Graphics::IDevice> graphicsDevice;
			std::unique_ptr<Graphics::IContext> graphicsContext;
			std::unique_ptr<Graphics::ISwapChain> swapChain;

		public:
			Main() {}
			~Main() {}

		public:
			bool Initialize(const wchar_t* name, s32 width, s32 height, uintPtr instance) override;
			void Finalize() override;
			void Run() override;

		public:
			virtual void Update() = 0;
			virtual void Render() = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix