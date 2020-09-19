#pragma once

#include <lua.hpp>
#include <memory>
#include "Phoenix\Graphics\GraphicsDevice.h"


namespace Phoenix
{
	namespace FrameWork
	{
		class LuaSystem
		{
		private:
			lua_State* luaState = nullptr;
			bool onFunction = false;
			s32 argNum = 0;

		public:
			LuaSystem() {}
			~LuaSystem() {}

		public:
			// ����
			static std::unique_ptr<LuaSystem> Create();

			// ������
			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice);

			// �I����
			void Finalize();

			// Lua�t�@�C�����J���ēǂݍ���
			bool LoadScript(const s8* scriptFileName);

			// �֐��J�n
			void BeginFunction(const s8* functionName);

			// �֐������s
			bool CallFunction(s32 returnValue);

			// �֐��I��
			void EndFunction();

			// Int�^�����̐ݒ�
			void SetIntArgument(s32 arg);

			// �O���[�o���ϐ��̎擾
			template<class T>
			T GetGlobalValue(const s8* argName)
			{
				if (!onFunction) return T();

				lua_getglobal(luaState, argName);

				T val;

				int type = lua_type(luaState, 1);
				switch (type)
				{
				case LUA_TNUMBER:
					val = static_cast<T>(lua_tonumber(luaState, 1));
					break;

				default: break;
				}

				lua_pop(luaState, 1);

				return val;
			}
		};
	}
}