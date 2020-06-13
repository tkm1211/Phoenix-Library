#pragma once
#include <memory>
#include "Phoenix/OS/Display.h"
#include "Phoenix/FrameWork/Main.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/Graphics/Context.h"
#include "Phoenix/Graphics/SwapChain.h"


class Main : public Phoenix::FrameWork::Main
{
private:
	using Super = Phoenix::FrameWork::Main;

public:
	Main() {}
	~Main() {}

public:
	bool Initialize(const wchar_t* name, Phoenix::s32 width, Phoenix::s32 height, Phoenix::uintPtr instance) override;
	void Finalize() override;
	void Update() override;
	void Render() override;
};