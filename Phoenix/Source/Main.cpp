#include "Main.h"


namespace Phoenix
{
	namespace OS
	{
		std::unique_ptr<IMain> IMain::Create()
		{
			return std::make_unique<Main>();
		}
	}
}

bool Main::Initialize(const wchar_t* name, Phoenix::s32 width, Phoenix::s32 height, Phoenix::uintPtr instance)
{
	Super::Initialize(name, width, height, instance);

	int i = 0;
	i++;

	return true;
}

void Main::Finalize()
{
	Super::Finalize();
}

void Main::Update()
{
	
}

void Main::Render()
{

}