#include "pch.h"
#include "Phoenix\FrameWork\Script\Lua.h"
#include "Phoenix\FND\Logger.h"
#include "Phoenix\OS\Path.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// ����
		std::unique_ptr<LuaSystem> LuaSystem::Create()
		{
			return std::make_unique<LuaSystem>();
		}

		// ������
		bool LuaSystem::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			luaState = nullptr;
			luaState = luaL_newstate();

			onFunction = false;
			argNum = 0;

			return true;
		}

		// �I����
		void LuaSystem::Finalize()
		{
			lua_close(luaState);
			luaState = nullptr;
		}

		// Lua�t�@�C�����J���ēǂݍ���
		bool LuaSystem::LoadScript(const s8* scriptFileName)
		{
			if (luaL_dofile(luaState, scriptFileName))
			{
				PHOENIX_LOG_GRP_ERROR("luaL_dofile : Failed!!\n");
				return false;
			}

			return true;
		}

		// �֐��J�n
		void LuaSystem::BeginFunction(const s8* functionName)
		{
			// Lua�X�e�[�g���ɂ���֐����w��
			lua_getglobal(luaState, functionName);

			onFunction = true;
			argNum = 0;
		}

		// �֐������s
		bool LuaSystem::CallFunction(s32 returnValue)
		{
			if (!onFunction) return false;

			// Lua�֐������s
			if (lua_pcall(luaState, argNum, returnValue, 0))
			{
				PHOENIX_LOG_GRP_ERROR("lua_pcall : Failed!!\n");
				return false;
			}

			if (0 < returnValue)
			{
				// �߂�l���擾
				for (s32 i = 1; i < returnValue; ++i)
				{
					int val = (int)lua_tonumber(luaState, i);
				}

				// TODO : �߂�l�̌^
				// TODO : �߂�l��Ԃ�
			}

			return true;
		}

		// �֐��I��
		void LuaSystem::EndFunction()
		{
			if (!onFunction) return;

			// �X�^�b�N���폜
			lua_pop(luaState, argNum);
		}

		// Int�^�����̐ݒ�
		void LuaSystem::SetIntArgument(s32 arg)
		{
			if (!onFunction) return;

			// ������ݒ�
			lua_pushnumber(luaState, arg);

			argNum++;
		}
	}
}
